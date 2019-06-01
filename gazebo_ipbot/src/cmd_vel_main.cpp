#include "cmd_vel.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "cmd_vel");
	ros::NodeHandle nh;

	cmd_vel v(nh);

	ros::spin();
	return 0;
}
