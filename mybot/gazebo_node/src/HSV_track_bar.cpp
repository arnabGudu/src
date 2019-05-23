
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;
using namespace cv;

sensor_msgs::ImagePtr msg_pub;
int h_hi,h_l,s_hi,s_l,v_hi,v_l;

void imageCallback(const sensor_msgs::ImageConstPtr& msg_sub){
try
   {
    Mat oframe=cv_bridge::toCvShare(msg_sub, "bgr8")->image;
    Mat frame;
    cvtColor(oframe,frame,COLOR_BGR2HSV);
    inRange(frame,Scalar(h_l,s_l,v_l),Scalar(h_hi,s_hi,v_hi),frame);
    cout<<h_hi<<" "<<h_l<<endl;
    imshow("view",frame);
    waitKey(30);
   }
catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg_sub->encoding.c_str());
   }
}

int main(int argc, char **argv)
{   //NODE,PUBS,SUBS
	ros::init(argc, argv, "HSV_track_bar");
	ros::NodeHandle nh;
	namedWindow("view"); //
	startWindowThread(); //
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("/camera/image", 1, imageCallback);

    //Trackbars
    h_hi = 0;h_l = 0;s_hi = 0;s_l = 0;v_hi = 0;v_l = 0;
    namedWindow("HSV_Value", 1);
    createTrackbar( "Hue_H", "HSV_Value", &h_hi, 180);
    createTrackbar( "Hue_L", "HSV_Value", &h_l, 180);
    createTrackbar( "Sat_H", "HSV_Value", &s_hi, 255);
    createTrackbar( "Sat_L", "HSV_Value", &s_l, 255);
    createTrackbar( "Val_H", "HSV_Value", &v_hi, 255);
    createTrackbar( "val_L", "HSV_Value", &v_l, 255);

    while(ros::ok)
	ros::spinOnce();
	cv::destroyWindow("view");//
    return 0;
}
