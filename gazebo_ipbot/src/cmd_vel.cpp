#include "cmd_vel.h"

cmd_vel::cmd_vel(ros::NodeHandle _nh) : nh(_nh)
{
	lastError = 0, lastAngle = 0;

	pub = nh.advertise<geometry_msgs::Twist>("qt_pi/cmd_vel", 1000);
	subPid = nh.subscribe("pid", 1000, &cmd_vel::callbackPid, this);
	subTuning = nh.subscribe("tuning", 1000, &cmd_vel::callbackTuning, this);
}

void cmd_vel::callbackPid(const gazebo_ipbot::pid::ConstPtr& _msg)
{
	int error = _msg->linear.data;
	int diff  = error - lastError;
	float balance = (float)(lkp * error + lkd * diff) / 1000;
	lastError = error;

	int angle = _msg->angular.data;
	int diffAng = angle - lastAngle;
	balance += (float)(akp * angle + akd * diffAng) / 1000;

	cout<<"bal"<<balance<<endl;

	msg.angular.z = balance;
	msg.linear.x = (float)(vel - akp * angle) / 1000;
	pub.publish(msg);
}

void cmd_vel::callbackTuning(const gazebo_ipbot::tuning::ConstPtr& _msg)
{
	lkp = _msg->lkp.data;
	lkd = _msg->lkd.data;
	lki = _msg->lki.data;
	akp = _msg->akp.data;
	akd = _msg->akd.data;
	aki = _msg->aki.data;
	vel = _msg->vel.data;
}
