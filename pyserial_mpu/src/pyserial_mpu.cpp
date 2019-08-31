#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace std;

void callback_msg(const std_msgs::String::ConstPtr& msg)
{
	string str = msg->data;
	

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "sub_pyserial");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("mpu", 1000, callback_msg);
	ros::spin();
}


