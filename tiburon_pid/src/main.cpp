#include "tiburon_pid.h"

using namespace std;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "pid");
	ros::NodeHandle nh;
	pid p(nh);
	ros::spin();
}
