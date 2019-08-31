#include <thruster_controller/ThrusterSpeeds.h>
#include "ros/ros.h"
using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "node");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<thruster_controller::ThrusterSpeeds>("/thruster_speeds", 1);
	ros::Rate loop_rate(10);
 
	int16_t count = 0;
	while(ros::ok())
	{
		thruster_controller::ThrusterSpeeds msg;
		
		for(int16_t i = 0; i < 6; i++)
		{	
			msg.data.push_back(0);
		}

		for(int16_t i = 0; i < 6; i++)
		{	
			msg.data[i] = i;
		}

		cout<<msg<<endl;

		//pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		count++;
	}
	return 0;

}

