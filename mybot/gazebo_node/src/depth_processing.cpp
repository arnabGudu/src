
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
image_transport::Publisher img_pub;
int l_thr,h_thr;

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


void detectionCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImageConstPtr cv_ptr;
  sensor_msgs::ImagePtr img_msg;
  try
  {
    cv_ptr = cv_bridge::toCvShare(msg);

  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }

  Mat mono8_img = cv::Mat(cv_ptr->image.size(), CV_8UC1);
  Mat mono8_img_l = mono8_img;//cv::Mat(cv_ptr->image.size(),CV_8UC1);
  Mat mono8_img_h = mono8_img;
  Mat mono8_img_binary = mono8_img;
  cv::convertScaleAbs(cv_ptr->image, mono8_img, 100, 0.0);
  // cv::imshow("Greyscale",mono8_img);
  // l_thr = 30;
  // h_thr = 50;
  // cv::threshold( mono8_img_l, mono8_img, l_thr,255,1);
  // cv::threshold( mono8_img_h, mono8_img, h_thr,255,1);
  // mono8_img_binary = mono8_img_h - mono8_img_l;
  cv::imshow("Greyscale",mono8_img);
  // cv::imshow("test",mono8_img_h);
  img_msg = cv_bridge::CvImage(std_msgs::Header(),"mono8",mono8_img).toImageMsg();
  img_pub.publish(img_msg);
  // moment(mono8_img);
  cv::waitKey(3);

}


int main(int argc, char **argv)
{

	ros::init(argc, argv, "depth_processing");
	ros::NodeHandle nh;
	cv::namedWindow("Greyscale");
  cv::namedWindow("Test");
    // cv::resizeWindow("view", 300,300);//
	cv::startWindowThread();
	image_transport::ImageTransport it(nh);
  img_pub = it.advertise("/camera/depth/greyscale",1);
  image_transport::Subscriber sub = it.subscribe("/camera/depth/image_depth", 1, detectionCallback);
  // l_thr=0;h_thr=0;
  // namedWindow("Thr_Value", 1);
  // createTrackbar( "Thr_H", "Thr_Value", &h_thr, 180);
  // createTrackbar( "Thr_L", "Thr_Value", &l_thr, 180);
    ros::spin();
    cv::destroyWindow("view");
    return 0;
}
