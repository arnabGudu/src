#include <thruster_controller/ThrusterSpeeds.h>
#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace std;

#define ROLL 0
#define PITCH 1
#define YAW 2
#define DEPTH 3
#define SURGE 4
#define SWAY 5

thruster_controller::ThrusterSpeeds msg_speed;
ros::Publisher pub;

void exec_cmd(int cmd)
{
	int16_t Forces[6] = {0,0,0,0,0,0};
	
	switch (cmd)
	{
		case 0:
			Forces[0] = 1500;
			Forces[1] = 1500;
			Forces[2] = 1500;
			Forces[3] = 1500;
			Forces[4] = 1500;
			Forces[5] = 1500;
			break;
		case 1:
			Forces[3] = 1600;
			Forces[4] = 1600;
			break;
		case -1:
			Forces[3] = 1400;
			Forces[4] = 1400;
			break;
		case 2:
			Forces[5] = 1600;
			break;
		case -2:
			Forces[5] = 1400;
			break;
		case 3:
			Forces[0] = 1600;
			Forces[1] = 1600;
			Forces[2] = 1600;
			break;
		case -3:
			Forces[0] = 1500;
			Forces[1] = 1500;
			Forces[2] = 1500;
			break;
		case 4:
			Forces[3] = 1600;
			Forces[4] = 1400;
			break;
		case -4:
			Forces[3] = 1400;
			Forces[4] = 1600;
			break;
	}
	
	for (int i = 0; i < 6; i++)
		msg_speed.data[i] = (int16_t)Forces[i];

	pub.publish(msg_speed);
}

void callback_key(const std_msgs::String::ConstPtr &msg)
{
	string str = msg->data;
	int cmd = 0;
	
	if (!str.find('W'))
		cmd = 1;
	else if (!str.find('S'))
		cmd = -1;
	if (!str.find('A'))
		cmd = 2;
	else if (!str.find('D'))
		cmd = -2;
	if (!str.find('Z'))
		cmd = 3;
	else if (!str.find('X'))
		cmd = -3;
	if (!str.find('Q'))
		cmd = 4;
	else if (!str.find('E'))
		cmd = -4;

	exec_cmd(cmd);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "node");
	ros::NodeHandle nh;
		
	ros::Subscriber sub_key = nh.subscribe("/key", 1, &callback_key);
	
	pub = nh.advertise<thruster_controller::ThrusterSpeeds>("thruster_speeds", 10); 

	for(int i = 0; i < 6; i++)
		msg_speed.data.push_back(1500);
	
	ros::spin();
}


