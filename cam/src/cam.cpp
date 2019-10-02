#include "cam.h"

cam::cam(int argc, char** argv, ros::NodeHandle _nh) : it(_nh), nh(_nh)
{
	sub = it.subscribe(argv[1], 1, &cam::callback, this);
}

void cam::callback(const sensor_msgs::ImageConstPtr& _msg)
{
	src = cv_bridge::toCvShare(_msg, "bgr8")->image;
	show();
	waitKey(10);
}

void cam::show()
{
	imshow("src", src);
}
