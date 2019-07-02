#ifndef CMD_VEL_H
#define CMD_VEL_H

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int32.h"
#include "opencv2/opencv.hpp"
#include "gazebo_ipbot/pid.h"
#include "gazebo_ipbot/tuning.h"

using namespace cv;
using namespace std;

class cmd_vel
{
public:
	cmd_vel(ros::NodeHandle _nh);
	void callbackPid(const gazebo_ipbot::pid::ConstPtr& _msg);
	void callbackTuning(const gazebo_ipbot::tuning::ConstPtr& _msg);

private:
	ros::NodeHandle nh;
	ros::Publisher pub;
	ros::Subscriber subTuning;
	ros::Subscriber subPid;
	geometry_msgs::Twist msg;

	float lastError, lastAngle, integral;
	float lkp, lki, lkd, akp, akd, aki, vel;
};

#endif
