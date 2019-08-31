#include "tiburon_pid.h"

#define ROLL 0
#define PITCH 1
#define YAW 2
#define DEPTH 3
#define SURGE 0
#define SWAY 1

#define tMin 0.01

using namespace std;

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
	}

	setpoint[DEPTH] = 837.031;
	thrust[0] = 1500;
	thrust[1] = 1500;
	
	pub = nh.advertise<thruster_controller::ThrusterSpeeds>("thruster_speeds", 10);
	pub_rqt = nh.advertise<geometry_msgs::Vector3>("rqt", 10);
		
	sub_vecnav = nh.subscribe("/combined", 1, &pid::callback_vecnav, this);
	sub_key = nh.subscribe("/key", 1, &pid::callback_key, this);	
	sub_pid = nh.subscribe("/pid", 10, &pid::callback_pid, this);	

	///////////////very important///////////////////
	for(int i = 0; i < 6; i++)
		msg_speed.data.push_back(1500);
}

///////////////////////////////////////////////////////////////////
//////////////////////PRINT FUNCTIONS//////////////////////////////
///////////////////////////////////////////////////////////////////

void Print(float arr[])
{
	for (int i = 0; i < 4; i++)
	{
		cout<<i<<" : "<<arr[i]<<"\t";
	}
	cout<<endl;
}

void pid::print()
{
	cout<<"yS:"<<setpoint[YAW]<<"  yM:"<<measure[YAW]<<"  yErr:";
	//cout<<"  dS:"<<setpoint[DEPTH]<<"  dM:"<<measure[DEPTH]<<"  dErr:"<<setpoint[DEPTH] - measure[DEPTH];
	//cout<<endl;
	
}

///////////////////////////////////////////////////////////////////
///////////////////CALLBACK FUNCTIONS//////////////////////////////
///////////////////////////////////////////////////////////////////

void pid::callback_vecnav(const synchronizer::Combined::ConstPtr &msg)
{
	measure[ROLL]	= msg->angular[0] * M_PI / 180.0; // - roll_tare;
	measure[PITCH]	= msg->angular[1] * M_PI / 180.0; // - pitch_tare;
	measure[YAW] 	= msg->angular[2];
	measure[DEPTH] 	= msg->depth;
	
	pid_control();
	thruster_speed();
}

void pid::callback_key(const std_msgs::String::ConstPtr &msg)
{
	string str = msg->data;

	if (!str.find('W'))
		thrust[SURGE] = 1600; 
		
	else if (!str.find('S'))
		thrust[SURGE] = 1400;
	
	else if (!str.find('A'))
		thrust[SWAY] = 1400;

	else if (!str.find('D'))
		thrust[SWAY] = 1600;

	else if (!str.find('Z'))
		setpoint[DEPTH] += 10;

	else if (!str.find('X'))
		setpoint[DEPTH] -= 10;

	else if (!str.find('Q'))
		setpoint[YAW] += 1;

	else if (!str.find('E'))
		setpoint[YAW] -= 1;

	else if (!str.find('_'))
	{
		thrust[SURGE] = 1500;
		thrust[SWAY] = 1500;	
	}	
}

void pid::callback_pid(const tiburon_controller::pid_tuning::ConstPtr &msg)
{
	int index = msg->mode;
	kp[index] = (msg->kp * 1.0) / 100;
	kd[index] = (msg->kd * 1.0) / 100;
	ki[index] = (msg->ki * 1.0) / 100;
}

///////////////////////////////////////////////////////////////////
////////////////////////PID FUNCTIONS//////////////////////////////
///////////////////////////////////////////////////////////////////

void pid::pid_control()
{
	if (setpoint[DEPTH] <= 837.031)
		setpoint[DEPTH] = 837.031;


	for (int i = 0; i < 4; i++) 
	{
		float error = setpoint[i] - measure[i];

		if (i == 2)
		{
			kp[2] = 0.001;
			if (error > 180)
			{
				error = error - 360;
			}
			print();
			cout<<error<<endl;
		}
		float diff = error - lastError[i];
		
		intg[i] += error;
		
		if (intg[i] > 2.55)
			intg[i] = 2.55;
		else if (intg[i] < -2.55)
			intg[i] = -2.55;
			
		lastError[i] = error;	
		
		balance[i] = kp[i] * error + ki[i] * intg[i] + kd[i] * diff;
	}

	//print();
	//Print(balance);
	//Print(setpoint);
}

	
void pid::thruster_speed()
{
	Forces[0] = 1500 + balance[DEPTH] - balance[PITCH] - balance[ROLL];
	Forces[1] = 1500 + balance[DEPTH] - balance[PITCH] + balance[ROLL];
	Forces[2] = 1500 + balance[DEPTH] + 2 * balance[PITCH];

	Forces[3] = thrust[SURGE] + balance[YAW];
	Forces[4] = thrust[SURGE] - balance[YAW];
	
	if (!thrust[SURGE])
	{
		Forces[3] = 1500 + balance[YAW];
		Forces[4] = 1500 - balance[YAW];
		cout<<"ooo"<<endl;
	}
	Forces[5] = thrust[SWAY];
	
	for (int i = 0; i < 6; i++)
	{
		if (Forces[i] > 1750)
			Forces[i] = 1750;
		else if (Forces[i] < 1500)
			if (i <= 2)
				Forces[i] = 1500;
			else
				Forces[i] = 1250;
	}
	
	for (int i = 0; i < 6; i++)
		msg_speed.data[i] = (int16_t)Forces[i];

	pub.publish(msg_speed);
}
