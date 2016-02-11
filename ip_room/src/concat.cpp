#include <opencv2/opencv.hpp>
#include "ros/ros.h"
#include "std_msgs/Int32.h"

using namespace std;
using namespace cv;

Mat src; 
int thresh_dist = 150, y_1 = 0, x_1 = 0, x_2 = 0, y_2 = 0;
int canny_thresh = 46;

float k = 2.0;
int times = 2;
int thresh_thresh = 30;

bool find_dist(vector<Point> cnt1, vector<Point> cnt2)
{
	Moments mu1, mu2;
	Point2f mc1, mc2;
	
	mu1 = moments(cnt1);
	mu2 = moments(cnt2);
	
	mc1 = Point2f( mu1.m10/mu1.m00, mu1.m01/mu1.m00 );
	mc2 = Point2f( mu2.m10/mu2.m00, mu2.m01/mu2.m00 );

	float dist = sqrt(pow(mc1.x - mc2.x, 2) + pow(mc1.y - mc2.y, 2));
	if (dist < thresh_dist)
		return true;
	else
		return false;	
}

bool find_pos(Point p, Point l1, Point l2)
{
	if (l2.x - l1.x != 0)
	{
		float side = (p.y - l1.y) - float(l2.y - l1.y)/float(l2.x - l1.x) * (p.x - l1.x);
	
		if (side >= 0)
			return true;
		else
			return false;
	}
}
	
int main(int argc, char** argv)
{
	ros::init(argc, argv, "talker");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<std_msgs::Int32>("camera", 1000);

	Mat gray, frameDelta, thresh, firstFrame;

	VideoCapture cap(argv[1]);

	int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
	int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 

	// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
	VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height)); 


	cap >> src;

	resize(src, src, Size(), k/2, k/2, CV_INTER_AREA);
	cvtColor(src, firstFrame, COLOR_BGR2GRAY);
	GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
	
	namedWindow("src");
	createTrackbar("thresh", "src", &thresh_thresh, 255, NULL);
	createTrackbar("thresh_dist", "src", &thresh_dist, 255, NULL);
	createTrackbar("times", "src", &times, 100, NULL);
	createTrackbar("canny", "src", &canny_thresh, 500, NULL);
	createTrackbar("x_1", "src", &x_1, src.size().width, NULL);
	createTrackbar("y_1", "src", &y_1, src.size().height, NULL);
	createTrackbar("x_2", "src", &x_2, src.size().width, NULL);
	createTrackbar("y_2", "src", &y_2, src.size().height, NULL);
						
		
	Point lines[4][2] ={{Point(000*k,184*k), Point(640*k,275*k)},
						{Point(000*k,138*k), Point(322*k,117*k)},
						{Point(322*k,117*k), Point(640*k,146*k)},
				
						{Point(000*k,288*k), Point(322*k,117*k)}};

	while(cap.isOpened() && ros::ok())
	{
		cap >> src;
		
		if (src.empty())
			break;
			
		resize(src, src, Size(), k/2, k/2, CV_INTER_AREA);
		cvtColor(src, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(21, 21), 0);

		absdiff(firstFrame, gray, frameDelta);
		threshold(frameDelta, thresh, thresh_thresh, 255, THRESH_TRUNC);
		Canny(thresh, thresh, canny_thresh, canny_thresh * 2, 3);

		dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
		erode(thresh, thresh, Mat(), Point(-1,-1), 2);
		
		vector<vector<Point> > cnts;
		
		findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		int length = cnts.size();
		int status[length];
		
		//cout<<"length: "<<length<<endl;

		for (int i = 0; i < length; i++)
			status[i] = 0;
		
		for (int i = 0; i < length; i++)
		{
			for (int j = i + 1; j < length; j++)
			{
				if (find_dist(cnts[i], cnts[j]))
				{
					int val = min(status[i], status[j]);
					status[i] = val;
					status[j] = status[i];
				}
				else
				{
					if (status[j] == status[i])
						status[j] = i + 1;
				}
			}
			//cout<<status[i]<<"\t";
		}
		//cout<<endl;
		
		int max = 0;
		for (int i = 0; i < length; i++)
			if (status[i] > max)
				max = status[i];
		max++;	//if max = 0 then size of mat must be 1
		//cout<<max<<endl;

		vector<vector<Point> > contours(max);

		//cvtColor(thresh, src, COLOR_GRAy_2BGR);
		for (int i = 0; i < max; i++)
		{
			for (int j = 0; j < length; j++)
				if (status[j] == i)
					contours[i].insert(contours[i].end(), cnts[j].begin(), cnts[j].end());	
					
			Rect boundRect = boundingRect(contours[i]);

			if (float(boundRect.height)/float(boundRect.width) < 1)
				continue;

			rectangle(src, boundRect, Scalar(255, 0, 0), 2);
			
			/*
			Point pos(boundRect.x + boundRect.width/2, boundRect.y + boundRect.height); 
			circle(src, pos, 2, Scalar(0,255,0), 2);
			
			int sl[4];

			for (int k = 0; k < 4; k++)
			{
				sl[k] = find_pos(pos, lines[k][0], lines[k][1]);
			}
				
			int loc = 0;
			
			if (sl[0] == 1)
				loc = 2;
			else
				loc = 1;
				
			if (sl[1] == 0 || sl[2] == 0)
				continue;
				
			if (sl[3] == 0)
				loc = loc * -1;
			
			//cout<<loc<<"\t"<<count<<endl;
			//////////////////////////ros///////////////////
			if (count > 100)
			{
				std_msgs::Int32 msg;
				msg.data = loc * 10 + i;
				pub.publish(msg);
				ros::spinOnce();
				last_loc = loc;
				count = 2001;
			}
			if (loc != last_loc)
			{
				last_loc = loc;
				count = 0;
			}	
			count++;
			*/			
		}
					
		line(src, Point(000*k,184*k), Point(640*k,275*k), Scalar(0,0,255), 2);
		line(src, Point(000*k,138*k), Point(322*k,117*k), Scalar(0,0,255), 2);
		line(src, Point(322*k,117*k), Point(640*k,146*k), Scalar(0,0,255), 2);

		line(src, Point(000*k,288*k), Point(322*k,117*k), Scalar(255,0,255), 2);
		
		line(src, Point(x_1,y_1), Point(x_2,y_2), Scalar(255,114,255), 2);
		

		imshow("src", src);
		//imshow("gray", gray);
		imshow("thresh", thresh);
		video.write(src);
		waitKey(40);
		
	}
	
}	
//thresh		40	70		105	98
//thresh_dist	81	123		92	93

//canny		46
//thresh	77
				
				
		
		
		
