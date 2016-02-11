#include <opencv2/opencv.hpp>
#include "ros/ros.h"
#include "std_msgs/Int32.h"

using namespace std;
using namespace cv;

Mat src; 
int thresh_dist = 150, y_1 = 0, x_1 = 0, x_2 = 0, y_2 = 0;

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
	cout<<"p:"<<p<<"\tl1:"<<l1<<"\tl2:"<<l2;
	if (l2.x - l1.x != 0)
	{
		float side = (p.y - l1.y) - (l2.y - l1.y)/(l2.x - l1.x)*(p.x - l1.x);
	
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

	cap >> src;

	float k = 1.0;
	int times = 2;
	int thresh_thresh = 25;
	int lowThreshold;
	int ratio = 3;
	int kernel_size = 3;

	resize(src, src, Size(), k/2, k/2, CV_INTER_AREA);
	cvtColor(src, firstFrame, COLOR_BGR2GRAY);
	GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
	
	namedWindow("src");
	createTrackbar("thresh", "src", &thresh_thresh, 255, NULL);
	createTrackbar("thresh_dist", "src", &thresh_dist, 255, NULL);
	createTrackbar("times", "src", &times, 100, NULL);
	createTrackbar("x_1", "src", &x_1, src.size().width, NULL);
	createTrackbar("y_1", "src", &y_1, src.size().height, NULL);
	createTrackbar("x_2", "src", &x_2, src.size().width, NULL);
	createTrackbar("y_2", "src", &y_2, src.size().height, NULL);
	
	Point lines[7][2] ={{Point(000*k,138*k), Point(198*k,138*k)},
						{Point(198*k,138*k), Point(480*k,186*k)},
						{Point(000*k,190*k), Point(480*k,190*k)},
						{Point(000*k,298*k), Point(480*k,292*k)},
					
						{Point(108*k,139*k), Point(000*k,251*k)},
						{Point(198*k,138*k), Point(149*k,360*k)},
						{Point(290*k,154*k), Point(407*k,360*k)}};
						
						
	while(cap.isOpened() && ros::ok())
	{
		cap >> src;
		
		if (src.empty())
			break;
			
		resize(src, src, Size(), k/2, k/2, CV_INTER_AREA);
		cvtColor(src, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gray, Size(21, 21), 0);

		absdiff(firstFrame, gray, frameDelta);
		threshold(frameDelta, thresh, thresh_thresh, 255, THRESH_BINARY);
		//Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );


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
			rectangle(src, boundRect, Scalar(255, 0, 0), 2);
			
			Point pos(boundRect.x + boundRect.width/2, boundRect.y + boundRect.height); 
			circle(src, pos, 2, Scalar(0,255,0), 2);
			
			int status_line;
			for (int i = 0; i < 7; i++)
			{
				if (find_pos(pos, lines[i][0], lines[i][1]))
					status_line = 1 + i;
				else
					status_line = 0;
					
				cout<<endl;
			}	
			cout<<"\t"<<status_line<<endl;
			
		}
					
		
		//line(src, Point(0,y), Point(src.size().width, y), Scalar(0,0,255), 2);
		//line(src, Point(x,0), Point(x, src.size().height), Scalar(255,0,255), 2);
		
		line(src, Point(0*k,138*k), Point(198*k, 138*k), Scalar(0,0,255), 2);
		line(src, Point(198*k,138*k), Point(480*k,186*k), Scalar(0,0,255), 2);
		line(src, Point(0*k,190*k), Point(480*k,190*k), Scalar(0,0,255), 2);
		line(src, Point(0*k,298*k), Point(480*k, 292*k), Scalar(0,0,255), 2);

		line(src, Point(108*k,139*k), Point(0*k, 251*k), Scalar(255,0,255), 2);
		line(src, Point(198*k,138*k), Point(149*k, 360*k), Scalar(255,0,255), 2);
		line(src, Point(290*k,154*k), Point(407*k, 360*k), Scalar(255,0,255), 2);
		
		line(src, Point(x_1,y_1), Point(x_2,y_2), Scalar(255,0,255), 2);
		
		imshow("src", src);
		//imshow("gray", gray);
		//imshow("thresh", thresh);
		waitKey(0);
		
		//////////////////////////ros///////////////////
		std_msgs::Int32 msg;
		msg.data = 10;
		pub.publish(msg);
		ros::spinOnce();		
	}
	
}	
		
		
				
				
		
		
		
