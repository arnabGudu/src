#include "pi_motor/pi_motor.h"

pi_motor::pi_motor(ros::NodeHandle _nh) : nh(_nh)
{
	pub = nh.advertise<geometry_msgs::Twist>("qt_pi/cmd_vel", 1000);
	sub_error = nh.subscribe("error", 1000, &pi_motor::callback_error, this);
	sub_pid = nh.subscribe("pid", 1000, &pi_motor::callback_pid, this);

	for (int i = 0; i < 2; i++)
	{
		p.kp.push_back(0);
		p.kd.push_back(0);
		p.ki.push_back(0);
		p.vel = 0;	
		
		lastError[i] = 0;
		intg[i] = 0;
	}
}

void pi_motor::callback_error(const pi_ipbot::error::ConstPtr& msg)
{
	int error[] = {msg->linear, msg->angular};
	
	pid(error); 
}

void pi_motor::callback_pid(const pi_ipbot::pid_pi::ConstPtr& msg)
{
	p = *msg;	
}

void pi_motor::pid(int error[])
{
	float balance[2];
	
	for (int i = 0; i < 2; i++) 
	{
		int diff = error[i] - lastError[i];
		intg[i] += error[i];
		
		lastError[i] = error[i];
		
		balance[i] = p.kp[i] * error[i] + p.kd[i] * diff + p.ki[i] * intg[i];
	}
	
	cout<<"lin: "<<balance[0]<<"  ang: "<<balance[1]<<endl;
	publish_twist(balance);
}

void pi_motor::publish_twist(float balance[])
{
	twist.linear.x = p.vel;
	twist.angular.z = balance[0];
	
	pub.publish(twist);
}

