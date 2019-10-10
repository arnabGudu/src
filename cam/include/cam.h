#ifndef CAM
#define CAM

#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"

using namespace cv;
using namespace std;

class cam
{
public:
	cam(int argc, char** argv, ros::NodeHandle _nh);
	void callback(const sensor_msgs::ImageConstPtr& _msg);

private:
	ros::NodeHandle nh;
	image_transport::ImageTransport it;
	image_transport::Subscriber sub;
	
	Mat src;
	
	void show();
	void video();
};

#endif
