#include <ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/Vector3.h>
#include<std_msgs/Float64.h>
using namespace std;
// Global Variables and Parameters


ros::Publisher pub;

float imageWidth = 800.0; //checked from img_pro_cal or you can simple check the
float imageHeight = 800.0; // check the size of the recieving node
geometry_msgs::Vector3 error;


void forward()
{
	float vel = 0.1;
	float kp = 0.001;
	geometry_msgs::Vector3 linar_values;
    linar_values.x = vel;
    linar_values.y = 0.0;
    linar_values.z = 0.0;

    geometry_msgs::Vector3 angular_values;
    angular_values.x = 0.0;
    angular_values.y = 0.0;
    angular_values.z = -kp*error.x;
	cout<<"F error in rotation:"<<angular_values.z<<"\n";
    geometry_msgs::Twist msg;
    msg.linear = linar_values;
    msg.angular = angular_values;
    pub.publish(msg);
}

void backward()
{

	float vel = 0.1;
	float kp = 0.001;

	ROS_INFO("Velocity:%f\n\n",vel);
	geometry_msgs::Vector3 linar_values;
    linar_values.x = -vel;
    linar_values.y = 0.0;
    linar_values.z = 0.0;


    geometry_msgs::Vector3 angular_values;
    angular_values.x = 0.0;
    angular_values.y = 0.0;
    angular_values.z = -kp*error.x;
	cout<<"B error in rotation:"<<angular_values.z<<"\n";
    geometry_msgs::Twist msg;
    msg.linear = linar_values;
    msg.angular = angular_values;
    pub.publish(msg);
}



void stop(std_msgs::Float64 area)
{
		cout<<"STOP";

		geometry_msgs::Vector3 linar_values;
		float area_ref = 17500;
		float area_error = area.data - area_ref;
		float kp = 0.1;
		linar_values.x = -kp*area_error/2500; //5000 inorder to make initial velocity 0.01 so
										// that in transition it doesnot stops suddenly
		linar_values.y = 0.0;
		linar_values.z = 0.0;

		geometry_msgs::Vector3 angular_values;
		angular_values.x = 0.0;
		angular_values.y = 0.0;
		angular_values.z = -0.01*error.x;

		geometry_msgs::Twist msg;
		msg.linear = linar_values;
		msg.angular = angular_values;


		pub.publish(msg);
}


void areaCallBack(std_msgs::Float64 area)
{	ROS_INFO("Area:%f\n",area);//
	if(area.data < 15000.0)
	{
		forward();
	}
	else if(area.data > 20000.0)
	{
		backward();
	}
	else
	stop(area);
}

void centerCallBack(geometry_msgs::Vector3 center)
{
	error.x = center.x - imageWidth/2;
	error.y = center.y - imageHeight/2; // here only x is used for code
	error.z = center.z - 0.0;
	cout<<"error in center:"<<error<<"\n";
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "motion_pub");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/pro_img",1000,areaCallBack);
	ros::Subscriber sub2 = nh.subscribe("/centre_coordinates",1000,centerCallBack);
	pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
	ros::spin();
	// ros::stop();
	return 0;

}
