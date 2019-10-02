#include "img_cap.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "image");
	ros::NodeHandle nh;
	image img(nh);
	ros::spin();
}
