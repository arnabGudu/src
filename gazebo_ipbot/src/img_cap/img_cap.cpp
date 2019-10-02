#include "img_cap.h"

//constructor
image::image(ros::NodeHandle _nh) : it(_nh), nh(_nh)
{
	//kp = 20; kd = 186; vel = 264;
	h = 0, s = 0, v = 0;
	H = 255, S = 255, V = 80;
	ht = 285, wd = 70;
	lkp = 20, lki = 0, lkd = 80, vel = 50;
	akp = 20, aki = 0, akd = 80;

	pubPid = nh.advertise<gazebo_ipbot::pid>("pid", 1000);
	pubTuning = nh.advertise<gazebo_ipbot::tuning>("tuning", 1000);
	sub = it.subscribe("front_camera/image_raw", 1, &image::callback, this);
	//trackbar();
	pidTrackbar();
}

//callback function
void image::callback(const sensor_msgs::ImageConstPtr& _msg)
{
	src = cv_bridge::toCvShare(_msg, "bgr8")->image;
	waitKey(10);
	perform();
}

void image::show()
{
	imshow("src", src);
	//imshow("blr", blr);
	//imshow("hsv", hsv);
	//imshow("thres", thres);
	//imshow("canny", canny);
}

void image::trackbar()
{
	namedWindow("thres");
	createTrackbar("h", "thres", &h, 255, NULL);
	createTrackbar("s", "thres", &s, 255, NULL);
	createTrackbar("v", "thres", &v, 255, NULL);
	createTrackbar("H", "thres", &H, 255, NULL);
	createTrackbar("S", "thres", &S, 255, NULL);
	createTrackbar("V", "thres", &V, 255, NULL);
}

void image::tuning()
{
	gazebo_ipbot::tuning msg;
	
	msg.lkp.data = lkp;
	msg.lkd.data = lkd;
	msg.lki.data = lki;

	msg.akp.data = akp;
	msg.akd.data = akd;
	msg.aki.data = aki;

	msg.vel.data = vel;

	pubTuning.publish(msg);
}

void image::pidTrackbar()
{
	namedWindow("src");
	createTrackbar("ht", "src", &ht, 487, NULL);
	createTrackbar("wd", "src", &wd, 487, NULL);

	createTrackbar("vel", "src", &vel, 1000, NULL);
	createTrackbar("lkp", "src", &lkp, 1000, NULL);
	createTrackbar("lkd", "src", &lkd, 1000, NULL);
	createTrackbar("lki", "src", &lki, 1000, NULL);

	createTrackbar("akp", "src", &akp, 1000, NULL);
	createTrackbar("akd", "src", &akd, 1000, NULL);
	createTrackbar("aki", "src", &aki, 1000, NULL);
}

//perform
void image::perform()
{
	blur(src, blr, Size(10, 10));
	cvtColor(src, hsv, CV_BGR2HSV);

	inRange(hsv, Scalar(h, s, v), Scalar(H, S, V), thres);

	Canny(thres, canny, 100, 200, 3);

	Rect r(120,ht,407,wd);

	vector< vector< Point > > contours;
	findContours(canny(r), contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector <RotatedRect> min(contours.size());
	int avgX = 0;
	int angle = 0;

	//find minAreaRect
	for (int i = 0; i < contours.size(); i++)
	{
		min[i] = minAreaRect(contours[i]);
	}

	//draw minAreaRect 
	for (int i = 0; i < contours.size(); i++)
	{
		Point2f rect_points[4];
		min[i].points(rect_points);
		for(int j = 0; j < 4; j++)
		{
			line(src(r), rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 2);
		}

		cout<<"cX:"<<(int)min[i].center.x + 120;
		cout<<" cY:"<<(int)min[i].center.y + ht;
		//cout<<" angle:"<<(int)min[i].angle<<endl;

		if (min[i].size.width < min[i].size.height)
		{
			angle += (int)min[i].angle;
			cout<<"angle1:"<<min[i].angle<<endl;
		}
		else
		{
			angle += (int)min[i].angle + 90;
			cout<<"angle2:"<<min[i].angle + 90<<endl;
		}
		avgX += (int)min[i].center.x;
	}

	rectangle(src, r, Scalar(0,0,255), 2);
	line(src, Point(323,0), Point(323, 487), Scalar(0, 255, 0), 2);

	gazebo_ipbot::pid msg;
	if (contours.size() > 0)
	{
		msg.linear.data = avgX / contours.size() + 120 - 323;
		msg.angular.data = angle / contours.size();
	}
	else
	{
		msg.linear.data = 0;
		msg.angular.data = 0;
	}
	pubPid.publish(msg);
	
	tuning();
	show();
}
