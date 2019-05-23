#include <ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/Vector3.h>
#include<std_msgs/Float64.h>

// Global Variables and Parameters


ros::Publisher pub;
// Add the center of the windom img.size() or similar
// error value calucation with resect to center modify
//check the PID code part if correct
double kp =0.01; //
double kd = 0.1; //
double ki = 0.0001; //
double Er[3] = {0, 0, 0};//
int imageWidth = 800; //checked from img_pro_cal or you can simple check the
int imageHeight = 800; // check the size of the recieving node

ros::Time lasttime ;
ros::Time currtime ;

void forward(double error){//
	ros::Duration(0.5).sleep();
	ROS_INFO("Error:%f\n",error);//
	Er[0] = Er[1];
	Er[1] = Er[2];
	Er[2] = error;
	currtime  = ros::Time::now();
	ros::Duration diff=currtime-lasttime;
	double T = diff.toSec();
	double vel;
	ROS_INFO("T:%f\n",T);
	vel = kp*(Er[2]-Er[1]) + (kd*(Er[2] - 2*Er[1] + Er[0]))/T + ki*Er[2]*T;
	if(vel<0)
	vel = 0.1;

	ROS_INFO("Velocity:%f\n\n",vel);
	geometry_msgs::Vector3 linar_values;
    linar_values.x = vel;
    linar_values.y = 0.0;
    linar_values.z = 0.0;

    geometry_msgs::Vector3 angular_values;
    angular_values.x = 0.0;
    angular_values.y = 0.0;
    angular_values.z = 0.0;

    geometry_msgs::Twist msg;
    msg.linear = linar_values;
    msg.angular = angular_values;
	lasttime  = ros::Time::now();
    pub.publish(msg);

}
void backward(double error){

	geometry_msgs::Vector3 linar_values;
	linar_values.x = -0.1;
	linar_values.y = 0.0;
	linar_values.z = 0.0;

	geometry_msgs::Vector3 angular_values;
	angular_values.x = 0.0;
	angular_values.y = 0.0;
	angular_values.z = 0.0;

	geometry_msgs::Twist msg;
	msg.linear = linar_values;
	msg.angular = angular_values;


	pub.publish(msg);
}

void backward(){

	geometry_msgs::Vector3 linar_values;
	linar_values.x = -0.1;
	linar_values.y = 0.0;
	linar_values.z = 0.0;

	geometry_msgs::Vector3 angular_values;
	angular_values.x = 0.0;
	angular_values.y = 0.0;
	angular_values.z = 0.0;

	geometry_msgs::Twist msg;
	msg.linear = linar_values;
	msg.angular = angular_values;


	pub.publish(msg);
}

void stop(){

	geometry_msgs::Vector3 linar_values;
	linar_values.x = 0.0;
	linar_values.y = 0.0;
	linar_values.z = 0.0;

	geometry_msgs::Vector3 angular_values;
	angular_values.x = 0.0;
	angular_values.y = 0.0;
	angular_values.z = 0.0;

	geometry_msgs::Twist msg;
	msg.linear = linar_values;
	msg.angular = angular_values;


	pub.publish(msg);
}

void subCallBack(std_msgs::Float64 area)
{	ROS_INFO("Area:%f\n",area);//
	if(area.data == 0.0 || area.data == 0.5) // if FULL Area is NOT discovered
	backward();
	else if(area.data < 39500.0)
	{	double error = 39500.0 - area.data;//
		forward(error); //
	}
	else if(area.data > 40500.0)
	{	double error = area.data - 40500.0 ; //
		backward(error); //
	}
	else
	stop();

}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "motion_pub");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/pro_img",1000,subCallBack);
	pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
	ros::spin();
	// ros::stop();
	return 0;

}
