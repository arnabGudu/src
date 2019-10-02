#include "pi_motor/pi_motor.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "pi_motor");
	ros::NodeHandle nh;

	pi_motor pi_m(nh);

	ros::spin();
	return 0;
}
