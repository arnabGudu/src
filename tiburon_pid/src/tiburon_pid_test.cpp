#include "tiburon_pid.h"

//DEPTH PID TEST//

pid::pid(ros::NodeHandle _nh) : nh(_nh)
{
	kp[0] = 0.025;
	kd[0] = 0.5;
	ki[0] = 0;
	
	pub = nh.advertise<thruster_controller::ThrusterSpeeds>("thruster_speeds", 10);
	sub_vecnav = nh.subscribe("/combined", 1, &pid::callback_vecnav, this);
	sub_key = nh.subscribe("/key", 1, &pid::callback_key, this);	
	sub_pid = nh.subscribe("/pid", 10, &pid::callback_pid, this);	

	intg[0] = 0;
	lastError[0] = 0;
	setpoint[0] = 838;

	for(int i = 0; i < 6; i++)
		msg_speed.data.push_back(0);
}

void pid::print()
{
	cout<<"measure: "<<measure[0];
	cout<<" set: "<<setpoint[0];
	cout<<" bal: "<<balance[0];
	cout<<" force:"<<Forces[0]<<","<<Forces[1]<<","<<Forces[2];
	cout<<endl;
}

///////////////////////////////////////////////////////////////////
///////////////////CALLBACK FUNCTIONS//////////////////////////////
///////////////////////////////////////////////////////////////////

void pid::callback_vecnav(const synchronizer::Combined::ConstPtr &_msg)
{
	measure[0] = _msg->depth;
	pid_control();
	thruster_speed();
}

void pid::callback_key(const std_msgs::String::ConstPtr &msg)
{
	string str = msg->data;
	
	if (str.find('W'))
		setpoint[0] += 1;
	else if (str.find('S'))
		setpoint[0] -= 1;
}

void pid::callback_pid(const tiburon_controller::pid_tuning::ConstPtr &msg)
{
	kp[0] = (msg->kp * 1.0) / 100;
	kd[0] = (msg->kd * 1.0) / 100;
	ki[0] = (msg->ki * 1.0) / 100;
}

///////////////////////////////////////////////////////////////////
////////////////////////PID FUNCTIONS//////////////////////////////
///////////////////////////////////////////////////////////////////

void pid::pid_control()
{
	float error = setpoint[0] - measure[0];
	float diff = error - lastError[0];
	
	intg[0] += error;
	
	if (intg[0] > 2.55)
		intg[0] = 2.55;
	else if (intg[0] < -2.55)
		intg[0] = -2.55;
		
	lastError[0] = error;	
	
	balance[0] = kp[0] * error + ki[0] * intg[0] + kd[0] * diff;
}

void pid::thruster_speed()
{
	Forces[0] = balance[0]; //- balance[PITCH] - balance[ROLL];
	Forces[1] = balance[0]; //- balance[PITCH] + balance[ROLL];
	Forces[2] = balance[0] * 2; //+ 2 * balance[PITCH];
	Forces[3] = 1500;
	Forces[4] = 1500;
	Forces[5] = 1500;
	
	for (int i = 0; i < 3; i++)
	{
		if (Forces[i] >= 0.01)
			  Forces[i] = (Forces[i]) * 370 / 2.36 + 1530;

		else if (Forces[i] <= (-0.01))
		  Forces[i] = 1470 + (Forces[i]) * 370 / 1.85;

		else
		  Forces[i] = 1500;

		if (Forces[i] > 1750)
		  Forces[i] = 1750;
		else if (Forces[i] < 1250)
		  Forces[i] = 1250;
	}

	for (int i = 0; i < 6; i++)
		msg_speed.data[i] = Forces[i];

	pub.publish(msg_speed);
	print();
}
