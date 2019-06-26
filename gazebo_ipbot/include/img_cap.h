#ifndef IMG_CAP_H
#define IMG_CAP_H

#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/Int32.h"
#include "gazebo_ipbot/pid.h"
#include "gazebo_ipbot/tuning.h"

using namespace cv;
using namespace std;

class image
{
public:
	image(ros::NodeHandle _nh);
	void callback(const sensor_msgs::ImageConstPtr& _msg);

private:
	ros::NodeHandle nh;
	ros::Publisher pubTuning;
	ros::Publisher pubPid;

	image_transport::ImageTransport it;
	image_transport::Subscriber sub;
	Mat src, blr, hsv, canny, thres;

	int h, s, v;
	int H, S, V;
	int ht, wd;
	int lkp, lkd, lki, vel;
	int akp, akd, aki;
	int index;

	void trackbar();
	void pidTrackbar();
	void show();
	void perform();
	void tuning();
	void error_calc(int input);	//only for img_cap_ard
};

#endif
