#include "cam.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "cam");
	ros::NodeHandle nh;
	cam c(argc, argv, nh);
	ros::spin();
}
