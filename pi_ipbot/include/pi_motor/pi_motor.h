#ifndef PI_MOTOR
#define PI_MOTOR 

#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include "geometry_msgs/Twist.h"
#include "pi_ipbot/pid_pi.h"
#include "pi_ipbot/error.h"

using namespace std;

class pi_motor
{
public:
	pi_motor(ros::NodeHandle _nh);
	void callback_error(const pi_ipbot::error::ConstPtr& _msg);
	void callback_pid(const pi_ipbot::pid_pi::ConstPtr& _msg);
	
private:
	ros::NodeHandle nh;
	ros::Subscriber sub_error;
	ros::Subscriber sub_pid;
	ros::Publisher pub;
	
	geometry_msgs::Twist twist;	
	pi_ipbot::pid_pi p;
	
	int lastError[2];
	int intg[2];
	
	void pid(int error[]);
	void publish_twist(float balance[]);
};
	
#endif
