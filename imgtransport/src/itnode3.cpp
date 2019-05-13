#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/Int32.h"

using namespace cv;

class Node3
{
public:
	Node3(ros::NodeHandle _nh);
	void publish(int sig);
	void node1Callback(const sensor_msgs::ImageConstPtr& msg1);
	void node2Callback(const sensor_msgs::ImageConstPtr& msg2);
private:
	ros::NodeHandle nh;
	std_msgs::Int32 msg;
	ros::Publisher pub;

	image_transport::ImageTransport it;
	image_transport::Subscriber sub1;
	image_transport::Subscriber sub2;
};
	
Node3::Node3(ros::NodeHandle _nh) : nh(_nh), it(_nh)
{
	pub = nh.advertise<std_msgs::Int32>("power", 1000);
	sub1 = it.subscribe("camera/image1", 1, &Node3::node1Callback, this);
	sub2 = it.subscribe("camera/image2", 1, &Node3::node2Callback, this);
}

void Node3::node1Callback(const sensor_msgs::ImageConstPtr& msg1)
{
	try 
	{
		Mat img1 = cv_bridge::toCvShare(msg1, "bgr8")->image;
		
		if (!img1.empty())
		{
			imshow("node_1", img1);
			int k = waitKey(10);
			if (k == ' ')	
				publish(0);
		}
	}
	catch (cv_bridge::Exception& e)
	{
		std::cout<<"could not receive node1 image"<<std::endl;
	}
}

void Node3::node2Callback(const sensor_msgs::ImageConstPtr& msg2)
{
        try 
	{	
		Mat img2 = cv_bridge::toCvShare(msg2, "mono8")->image;
                
		if (!img2.empty())
		{	
			imshow("node_2", img2);
                	int k = waitKey(10);
			if (k == ' ')	
				publish(0);
		}
	}
        catch (cv_bridge::Exception& e)
        {
		std::cout<<"could not receive node2 image"<<std::endl;
	}
}	

void Node3::publish(int sig)
{	
	msg.data = sig;
	pub.publish(msg);
}
	
int main(int argc, char **argv)
{
	ros::init(argc, argv, "node3");
	ros::NodeHandle nh;
	Node3 n(nh);
	ros::spin();
}
