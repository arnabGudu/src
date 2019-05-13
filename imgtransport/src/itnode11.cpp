#include "ros/ros.h"
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"

using namespace cv;

Mat src;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "node11");
	ros::NodeHandle nh;
	
	image_transport::ImageTransport it(nh);
	image_transport::Publisher pub = it.advertise("camera/image2", 1);
	
	//Mat image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	
	VideoCapture cap(argv[1]);
	ros::Rate r(10);
	
	while(ros::ok() && cap.isOpened())
	{	
		cap >> src;
		
		if (src.empty())
			break;

		sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", src).toImageMsg();
		
		int k = waitKey(10);
		if (k == 'q')
			return 0;	
		
		pub.publish(msg);
		ros::spinOnce();
		r.sleep();
	}
}
