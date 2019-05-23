
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include<std_msgs/Float64.h>
#include<geometry_msgs/Vector3.h>

using namespace std;
using namespace cv;

ros::Publisher pub;
ros::Publisher pub2;
sensor_msgs::ImagePtr msg_pub;

void moment(Mat img,Mat &oframe)
{

  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Canny( img, canny_output, 120, 120*2, 3 );
  findContours(canny_output, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  if(!contours.empty())
  {
  vector<double> areas(contours.size());
  for(int i=0;i<contours.size();++i)
  {    areas[i]=contourArea(contours.at(i),0);}

  Moments m;
  std_msgs::Float64 max; //
  max.data = *max_element( areas.begin(), areas.end() );//
  // data.area = max// pub.publish(max);

  for(int i=0;i<areas.size();++i)
  {
  if(max.data == areas[i])
  {    geometry_msgs::Vector3 center;
       m=moments(contours[i],false);
       Point2f ma = Point2f(m.m10/m.m00,m.m01/m.m00);
       center.x = (m.m10/m.m00);
       center.y = (m.m01/m.m00);
       center.z = 0;
       circle(oframe,ma,4,Scalar(0,0,255),-1);
       drawContours(oframe,contours,i,Scalar(0,255,0),2,8,hierarchy,0,Point());
       pub.publish(max);
       pub2.publish(center);
       cout<<center<<endl;
       break;
  }
  }

}
}


void imageCallback(const sensor_msgs::ImageConstPtr& msg_sub){
try
   {

    Mat oframe=cv_bridge::toCvShare(msg_sub, "bgr8")->image;
    //thresholding
    Mat frame;
    cv::Size sz = oframe.size();
    // int imageWidth = sz.width;
    // int imageHeight = sz.height;
    // ROS_ERROR("%d (Width) and %d (Hieght) ", imageWidth,imageHeight);
    cvtColor(oframe,frame,COLOR_BGR2HSV);
    inRange(frame,Scalar(0,0,0),Scalar(0,0,40),frame);
    moment(frame,oframe);
    imshow("view",oframe);
    waitKey(30);
   }
catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg_sub->encoding.c_str());
   }
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "raw_img_cap");
	ros::NodeHandle nh;
	cv::namedWindow("view");
    // cv::resizeWindow("view", 300,300);//
	cv::startWindowThread();
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("/mybot/camera1/image_raw", 1, imageCallback);
    pub = nh.advertise<std_msgs::Float64>("pro_img",1000);
    pub2 = nh.advertise<geometry_msgs::Vector3>("centre_coordinates",1000);//
    while(ros::ok)
	ros::spin();
	cv::destroyWindow("view");
    // stop();
    return 0;
}
