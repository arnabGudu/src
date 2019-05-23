#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int32.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class cmd_vel
{
public:
	cmd_vel(ros::NodeHandle _nh);
	void callbackErr(const std_msgs::Int32::ConstPtr& _msg);
	void callbackPid(const std_msgs::Int32::ConstPtr& _msg);
	
private:
	ros::NodeHandle nh;
	ros::Publisher pub;
	ros::Subscriber subErr;
	ros::Subscriber subPid;
	geometry_msgs::Twist msg;
	
	float lastError;
	int side, angle;
	int kp, ki, kd;
};

cmd_vel::cmd_vel(ros::NodeHandle _nh) : nh(_nh)
{
	angle = 0;  side = 0;
	lastError = 0;
	kp = 20;  ki = 260;  kd = 80;

	pub = nh.advertise<geometry_msgs::Twist>("qt_pi/cmd_vel", 1000);
	subErr = nh.subscribe("error", 1000, &cmd_vel::callbackErr, this);
	subPid = nh.subscribe("pid", 1000, &cmd_vel::callbackPid, this);
}

void cmd_vel::callbackErr(const std_msgs::Int32::ConstPtr& _msg)
{	
	int error = _msg->data;
	int diff  = error - lastError;
	float balance = (float)kp/1000 * error + (float)kd/1000 * diff;
	lastError = error;

	cout<<"bal"<<balance<<endl;

	msg.angular.z = balance;
	msg.linear.x = (float)ki/1000;
	pub.publish(msg);
}

void cmd_vel::callbackPid(const std_msgs::Int32::ConstPtr& _msg)
{	
	long int data = _msg->data;

	kp = data % 1000;
	data /= 1000;

	kd = data % 1000;
	data /= 1000;

	ki = data;

	waitKey(10);		
}

int main(int argc, char **argv)
{	
	ros::init(argc, argv, "cmd_vel");
	ros::NodeHandle nh;
	
	cmd_vel v(nh);
	
	ros::spin();
	return 0;	
}
