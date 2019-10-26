#ifndef PID
#define PID

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <synchronizer/Combined.h>
#include <thruster_controller/ThrusterSpeeds.h>
#include "tiburon_controller/pid_tuning.h"
#include <tiburon_pid/rqt.h>

#define ROLL 0
#define PITCH 1

#define LEFT 0
#define RIGHT 1
#define SWAY 2

#define HEAVE 3
#define YAW 2

#define tMin 0.01

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
	void print(float error, int i);

	float kp[4], kd[4], ki[4];
	float measure[4], setpoint[4];
	float intg[4], lastError[4];

	float Forces[6];
	float balance[4];
	int16_t thrust[4];
};

#endif
