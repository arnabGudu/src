#include "tiburon_pid.h"

pid::pid(ros::NodeHandle _nh) : nh(_nh)
{
	for (int i = 0; i < 4; i++)
	{
		kp[i] = 0;
		kd[i] = 0;
		ki[i] = 0;
		
		intg[i] = 0;
		lastError[i] = 0;
		setpoint[i] = 0;
		
		thrust[i] = 1500;
	}
	
	setpoint[HEAVE] = 837;
		
	pub = nh.advertise<thruster_controller::ThrusterSpeeds>("thruster_speeds", 10);
	pub_rqt = nh.advertise<tiburon_pid::rqt>("rqt", 10);
		
	sub_vecnav = nh.subscribe("/combined", 1, &pid::callback_vecnav, this);
	sub_key = nh.subscribe("/key", 1, &pid::callback_key, this);	
	sub_pid = nh.subscribe("/pid", 10, &pid::callback_pid, this);	

	///////////////very important///////////////////
	for(int i = 0; i < 6; i++)
		msg_speed.data.push_back(1500);
}

void pid::callback_vecnav(const synchronizer::Combined::ConstPtr &msg)
{
	measure[ROLL]	= msg->angular[0] * M_PI / 180.0; // - roll_tare;
	measure[PITCH]	= msg->angular[1] * M_PI / 180.0; // - pitch_tare;
	measure[YAW] 	= msg->angular[2];
	measure[HEAVE] 	= msg->depth;
	
	pid_control();
	thruster_speed();
}

void pid::callback_pid(const tiburon_controller::pid_tuning::ConstPtr &msg)
{
	for (int i = 0; i < 4; i++)
	{
		kp[i] = msg->kp[i];
		kd[i] = msg->kd[i];
		ki[i] = msg->ki[i];
		//thrust[i] = msg->sp[i];
	}
}

void pid::callback_key(const std_msgs::String::ConstPtr &msg)
{
	string str = msg->data;

	if (!str.find('W'))
	{
		thrust[LEFT] = 1600;
		thrust[RIGHT] = 1600;
	}
		
	else if (!str.find('S'))
	{
		thrust[LEFT] = 1400;
		thrust[RIGHT] = 1400;
	}
	/*
	else if (!str.find('A'))
	{
		thrust[LEFT] = 1425;
		thrust[RIGHT] = 1575;
		setpoint[YAW] = measure[YAW];
	}

	else if (!str.find('D'))
	{
		thrust[LEFT] = 1575;
		thrust[RIGHT] = 1425;
		setpoint[YAW] = measure[YAW];
	}
	*/
	
	else if (!str.find('A')) 
	{
		setpoint[YAW]--;
		if (setpoint[YAW] < -180)
			setpoint[YAW] = 180;
	}

	else if (!str.find('D'))
	{
		setpoint[YAW]++;
		if (setpoint[YAW] > 180)
			setpoint[YAW] = -180;
	}
	
	else if (!str.find('Q'))
		thrust[SWAY] = 1400;

	else if (!str.find('E'))
		thrust[SWAY] = 1600;

	else if (!str.find('Z')) 
	{
		setpoint[HEAVE]--;
		if (setpoint[HEAVE] < 837)
			setpoint[HEAVE] = 837;
	}

	else if (!str.find('X'))
	{
		setpoint[HEAVE]++;
	}
	
	/*
	else if (!str.find('Z'))
	{
		thrust[HEAVE] = 1650;
		setpoint[HEAVE] = measure[HEAVE];	
	}
		
	else if (!str.find('X'))
	{
		thrust[HEAVE] = 1500;
		setpoint[HEAVE] = measure[HEAVE];	
	}*/

	else if (!str.find('_'))
	{
		thrust[LEFT] = 1500;
		thrust[RIGHT] = 1500;
		thrust[SWAY] = 1500;	
	}	
}
