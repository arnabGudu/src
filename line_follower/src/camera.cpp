#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "geometry_msgs/Twist.h"

using namespace cv;

class camera	
{
public:
	camera(ros::NodeHandle _nh);
	void callback(const sensor_msgs::ImageConstPtr& msg);
private:
	image_transport::ImageTransport it;
	image_transport::Subscriber sub;
	Mat src;
};


camera::camera(ros::NodeHandle _nh) : it(_nh)
{
	sub = it.subscribe("front_camera/image_raw", 1, &camera::callback, this);
}

void camera::callback(const sensor_msgs::ImageConstPtr& msg)
{
	src = cv_bridge::toCvShare(msg, "bgr8")->image;
	imshow("camera", src);
	waitKey(10);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "cameraListener");
	ros::NodeHandle nh;

	camera c(nh);
	ros::spin();
}



