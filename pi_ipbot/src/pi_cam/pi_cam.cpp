#include "pi_cam/pi_cam.h"
 
pi_cam::pi_cam(ros::NodeHandle _nh) : it(_nh), nh(_nh)
{
	pub = nh.advertise<pi_ipbot::error>("error", 1000);
	sub = it.subscribe("front_camera/image_raw", 1, &pi_cam::callback, this);
	trackbar();
	
	thres_low = 55;
	area = 100;
	ht = 40;
	canny_thresh = 2;
}


void pi_cam::callback(const sensor_msgs::ImageConstPtr& _msg)
{
	src = cv_bridge::toCvShare(_msg, "bgr8")->image;
	trackbar();
	perform();
	show();
	waitKey(10);
}


void pi_cam::show()
{
	imshow("src", src);
	//imshow("blr", blr);
	//imshow("hsv", hsv);
	//imshow("thres", thres);
	//imshow("canny", canny);
}


void pi_cam::perform()
{
	resize(src, src, Size(), 1, 1, CV_INTER_AREA);
		
	Rect r(0, ht, src.cols, src.rows - ht);
	//rectangle(src, r, Scalar(255, 255, 0), 2);
	
	src = src(r);
	
	cvtColor(src, gray, CV_BGR2GRAY);
	blur(src, src, Size(1, 1));
	threshold(gray, thresh, thres_low, 255, THRESH_BINARY_INV);

	dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
	erode(thresh, thresh, Mat(), Point(-1,-1), 2);
	
	vector< vector< Point > > contours;
	findContours(thresh, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

	int cnt_size = contours.size();
	
	vector <RotatedRect> min(cnt_size);
	float mid = 0, angle = 0;
	
	for (int i = 0; i < cnt_size; i++)
	{
		if (contourArea(contours[i]) < area)
			continue;
		
		min[i] = minAreaRect(contours[i]);
		
		Point2f rect_points[4];
		min[i].points(rect_points);
		for(int j = 0; j < 4; j++)
		{
			line(src, rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 2);
		}

		if (min[i].size.width < min[i].size.height)
		{
			angle += (int)min[i].angle;
			cout<<"  ang: "<<(int)min[i].angle;
		}
		else 
		{
			angle += (int)min[i].angle + 90;
			cout<<"  ang: "<<(int)min[i].angle + 90;
		}
		mid += min[i].center.x;
		
		circle(src, Point(min[i].center.x, min[i].center.y), 2, Scalar(0,0,255), 2);
		//drawContours(src, contours, i, Scalar(0,255,0), 2);
	}
	
	mid /= cnt_size;
	
	float error = src.cols/2 - mid;
	cout<<"  lin: "<<error<<"  ang: "<<angle<<endl;
	
	publish_error(error, angle); 
	
	circle(src, Point(mid, src.rows/2), 2, Scalar(0,0,255), 2);
	line(src, Point(src.cols/2, 0), Point(src.cols/2, src.rows), Scalar(0, 0, 255), 2); 	
}

void pi_cam::publish_error(int linear, int angular)
{
	pi_ipbot::error msg;
	
	msg.linear = linear;
	msg.angular = angular;
	 
	pub.publish(msg);
}

void pi_cam::trackbar()
{
	namedWindow("thresh");
	namedWindow("src");
	createTrackbar("thres_low", "thresh", &thres_low, 255, NULL);
	createTrackbar("screen_ht", "thresh", &ht, 255, NULL);
	createTrackbar("cnts_area", "thresh", &area, 255, NULL);
	createTrackbar("cany_thrs", "thresh", &canny_thresh, 255, NULL);
}
