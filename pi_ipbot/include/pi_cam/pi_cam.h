#ifndef PI_CAM 
#define PI_CAM

#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "pi_ipbot/error.h"

using namespace cv;
using namespace std;

class pi_cam
{
public:
	pi_cam(ros::NodeHandle _nh);
	void callback(const sensor_msgs::ImageConstPtr& _msg);
	
private:
	ros::NodeHandle nh;
	ros::Publisher pub;
	
	image_transport::ImageTransport it;
	image_transport::Subscriber sub;
	
	Mat src, gray, thresh;
	
	void trackbar();
	void show();
	void perform();
	void publish_error(int linear, int angular);
	
	int thres_low, area, ht, canny_thresh;
};
	
#endif
