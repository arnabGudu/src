#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/Int32.h"
#include "gazebo_ipbot/pid.h"

using namespace cv;
using namespace std;

class image
{
public:
	image(ros::NodeHandle _nh);
	void callback(const sensor_msgs::ImageConstPtr& _msg);

private:
	ros::NodeHandle nh;
	ros::Publisher pubErr;
	ros::Publisher pubPid;

	image_transport::ImageTransport it;
	image_transport::Subscriber sub;
	Mat src, blr, hsv, canny, thres;

	int h, s, v;
	int H, S, V;
	int ht, wd;
	int kp, kd, ki, vel;

	void trackbar();
	void pidTrackbar();
	void show();
	void perform();	
};

//constructor
image::image(ros::NodeHandle _nh) : it(_nh), nh(_nh)
{
	h = 0;	s = 0;	v = 0;
	H = 255;  S = 255;  V = 80;
	ht = 285; wd = 70;
	kp = 20;  ki = 20;  kd = 80, vel = 50;

	//kp = 20; kd = 186; vel = 264;

	pubErr = nh.advertise<std_msgs::Int32>("error", 1000);
	pubPid = nh.advertise<gazebo_ipbot::pid>("pid", 1000);
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

void image::pidTrackbar()
{
	namedWindow("src");
	createTrackbar("ht", "src", &ht, 487, NULL);
	createTrackbar("wd", "src", &wd, 487, NULL);

	createTrackbar("vel", "src", &vel, 1000, NULL);
	createTrackbar("kp", "src", &kp, 1000, NULL);
	createTrackbar("kd", "src", &kd, 1000, NULL);
	createTrackbar("ki", "src", &ki, 1000, NULL);	
	
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "image");
	ros::NodeHandle nh;
	image img(nh);
	ros::spin();
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
			cout<<"angle1:"<<min[i].angle<<endl;
		else
			cout<<"angle2:"<<min[i].angle + 90<<endl;

		avgX += (int)min[i].center.x;
	}	

	rectangle(src, r, Scalar(0,0,255), 2);
	line(src, Point(323,0), Point(323, 487), Scalar(0, 255, 0), 2); 
	circle(src, Point(avgX, 200), 4, Scalar(255,0,0), 2);
	
	if (contours.size() > 0)
	{
		std_msgs::Int32 msgErr;
		gazebo_ipbot::pid msgPid;

		msgErr.data = avgX / contours.size() + 120 - 323;

		msgPid.kp.data = kp;
		msgPid.kd.data = kd;
		msgPid.ki.data = ki;
		msgPid.vel.data = vel;

		pubErr.publish(msgErr);
		pubPid.publish(msgPid);
	}
	else
	{
		std_msgs::Int32 msgErr;
		msgErr.data = 0;
		pubErr.publish(msgErr);
	}
		
	show();
}
