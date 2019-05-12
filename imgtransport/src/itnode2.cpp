#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"

using namespace cv;

void imageCallBack(const sensor_msgs::ImageConstPtr& msg)
{
	try
	{
		imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
		cv::waitKey(10);
	}
	catch (cv_bridge::Exception& e)
	{
		std::cout<<"could not convert image"<<std::endl;
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "node2");
	ros::NodeHandle nh;
	
	namedWindow("view");
	startWindowThread();

	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallBack);

	ros::spin();
	destroyWindow("view");
}
