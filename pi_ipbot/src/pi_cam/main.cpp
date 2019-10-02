#include "pi_cam/pi_cam.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "pi_cam");
	ros::NodeHandle nh;
	pi_cam pic(nh);
	ros::spin();
}
