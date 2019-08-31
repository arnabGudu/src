#ifndef PID
#define PID

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <synchronizer/Combined.h>
#include <thruster_controller/ThrusterSpeeds.h>
#include "tiburon_controller/pid_tuning.h"
#include <geometry_msgs/Vector3.h>

using namespace std;

class pid
{
public:
	pid(ros::NodeHandle _nh);
	void callback_vecnav(const synchronizer::Combined::ConstPtr &msg);
	void callback_key(const std_msgs::String::ConstPtr &msg);
	void callback_pid(const tiburon_controller::pid_tuning::ConstPtr &msg);

private:
	ros::NodeHandle nh;
	ros::Publisher pub, pub_rqt;
	ros::Subscriber sub_vecnav, sub_key, sub_pid;
	thruster_controller::ThrusterSpeeds msg_speed;
	
	void pid_control();
	void thruster_speed();
	void print();

	float kp[4], kd[4], ki[4];
	float measure[4], setpoint[4];
	float intg[4], lastError[4];

	float Forces[6];
	float balance[4];
	int16_t thrust[2];
};

#endif
