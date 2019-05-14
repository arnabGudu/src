#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/Int32.h"

using namespace cv;

class Node1
{
public:
	Mat src;
	Node1(ros::NodeHandle _nh);
	void Callback(const std_msgs::Int32::ConstPtr& _msg);

	bool ok()
	{
		return state;
	}

	void publish()
	{
		msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", src).toImageMsg();
		
		pub.publish(msg);
		
		std::cout<<state<<std::endl;
		waitKey(10);
	}
	
private:
	ros::NodeHandle nh;
	
	image_transport::ImageTransport it;
	image_transport::Publisher pub;
	sensor_msgs::ImagePtr msg;

	ros::Subscriber sub;
	bool state;
};

Node1::Node1(ros::NodeHandle _nh) : it(_nh), nh(_nh)
{
	state = 1;
	pub = it.advertise("camera/image1", 1);
	sub = nh.subscribe("power", 1000, &Node1::Callback, this);
}

void Node1::Callback(const std_msgs::Int32::ConstPtr& _msg)
{	
	state = _msg->data;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "node1");
	ros::NodeHandle nh;
	
	Node1 n(nh);
	//Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	
	VideoCapture cap(0);
	ros::Rate r(10);
	
	while(ros::ok() && cap.isOpened())
	{	
		cap >> n.src;
		
		if (n.src.empty())
			break;

		if (!n.ok())
			break;

		n.publish();
		ros::spinOnce();
		r.sleep();
	}
	return 0;
}
