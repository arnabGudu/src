#include "ros/ros.h"
#include "std_msgs/Int32.h"

int main(int argc, char** argv)
{
	ros::init(argc, argv, "camera_data");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<std_msgs::Int32>("camera", 1000);
	ros::Rate loop_rate(10);	
	
	bool state = false;

	while(ros::ok())
	{
		std_msgs::Int32 msg;
		if (state)
			msg.data = 1;
		else
			msg.data = 0;

		pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		
		state = !state;
	}
	return 0;
}
