#include "tiburon_pid.h"

void pid::pid_control()
{
	for (int i = 0; i < 4; i++) 
	{
		float error = setpoint[i] - measure[i];
		
		if (i == 2)
		{
			if (error > 180)
				error = error - 360;
			else if (error < -180)
				error = error + 360;
			 
		}
		print(error, i);
		float diff = error - lastError[i];
		
		intg[i] += error;
		
		if (intg[i] > 2.55)
			intg[i] = 2.55;
		else if (intg[i] < -2.55)
			intg[i] = -2.55;
			
		lastError[i] = error;	
		
		balance[i] = kp[i] * error + ki[i] * intg[i] + kd[i] * diff;
	}
	cout<<endl;
}

	
void pid::thruster_speed() 
{
	Forces[0] = thrust[HEAVE] + balance[HEAVE] - balance[PITCH] - balance[ROLL];
	Forces[1] = thrust[HEAVE] + balance[HEAVE] - balance[PITCH] + balance[ROLL];
	Forces[2] = thrust[HEAVE] + balance[HEAVE] + 2 * balance[PITCH];
		
	Forces[3] = thrust[LEFT] + balance[YAW];
	Forces[4] = thrust[RIGHT] - balance[YAW];
	
	//cout<<"  balTh: "<<balance[YAW]<<endl;
	Forces[5] = thrust[SWAY];
	
	for (int i = 0; i < 6; i++)
	{
		if (Forces[i] < 1530 && Forces[i] >= 0)
			Forces[i] += 30;
		else if (Forces[i] > -1530 && Forces[i] < 0)
			Forces[i] -= 30;
			
		msg_speed.data[i] = (int16_t)Forces[i];
	}
	pub.publish(msg_speed);

	tiburon_pid::rqt msg_rqt;
	msg_rqt.pitch = balance[PITCH];
	msg_rqt.roll = balance[ROLL];
	msg_rqt.yaw = balance[YAW];	
	msg_rqt.heave = balance[HEAVE];
	
	pub_rqt.publish(msg_rqt);
}


void Print(float arr[])
{
	for (int i = 0; i < 4; i++)
	{
		cout<<i<<" : "<<arr[i]<<"\t";
	}
	cout<<endl;
}

void Print(int16_t arr[])
{
	for (int i = 0; i < 4; i++)
	{
		cout<<i<<" : "<<arr[i]<<"\t";
	}
	cout<<endl;
}

void pid::print(float error, int i)
{
	if (i == 0)			cout<<"ROLL";
	else if (i == 1)	cout<<"PITCH";
	else if (i == 2)	cout<<"YAW";
	else if (i == 3)	cout<<"HEAVE";
	
	if (i != 3)
		cout<<"\tset: "<<setpoint[i]<<"\t\tm: "<<measure[i]<<"\t\terr: "<<error<<endl;	
	else
		cout<<"\tset: "<<setpoint[i]<<"\tm: "<<measure[i]<<"\t\terr: "<<error<<endl;	
}
