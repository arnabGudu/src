#include "cmd_vel.h"

cmd_vel::cmd_vel(ros::NodeHandle _nh) : nh(_nh)
{
	lastError = 0, lastAngle = 0;
	integral = 0;

	pub = nh.advertise<geometry_msgs::Twist>("qt_pi/cmd_vel", 1000);
	subPid = nh.subscribe("pid", 1000, &cmd_vel::callbackPid, this);
	subTuning = nh.subscribe("tuning", 1000, &cmd_vel::callbackTuning, this);
}

void cmd_vel::callbackPid(const gazebo_ipbot::pid::ConstPtr& _msg)
{
	int max_control, min_control;

	int error = _msg->linear.data;

	integral += error;
	if (integral >= max_control)
		integral = max_control;
	else if (integral <= min_control)
		integral = min_control;

	int derivative = error - lastError;
	
	float balance = lkp * error + lkd * derivative + lki * integral;
	
	lastError = error;

	//int angle = _msg->angular.data;
	//int diffAng = angle - lastAngle;
	//balance += (akp * angle + akd * diffAng);

	printf("kp %f\tki %f\tkd %f\n", lkp, lki, lkd);

	msg.angular.z = balance;
	msg.linear.x = vel;//(float)(vel - akp * angle) / 1000;
	pub.publish(msg);
}

void cmd_vel::callbackTuning(const gazebo_ipbot::tuning::ConstPtr& _msg)
{
	lkp = (float)_msg->lkp.data / 1000;
	lkd = (float)_msg->lkd.data / 1000;
	lki = (float)_msg->lki.data / 1000;
	akp = (float)_msg->akp.data / 1000;
	akd = (float)_msg->akd.data / 1000;
	aki = (float)_msg->aki.data / 1000;
	vel = (float)_msg->vel.data / 1000;
}
