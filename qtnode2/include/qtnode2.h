#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ros/ros.h>
#include "opencv2/opencv.hpp"
#include "image_transport/image_transport.h"
#include "cv_bridge/cv_bridge.h"
#include "std_msgs/Int32.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(ros::NodeHandle _nh,QWidget *parent = 0);
	~MainWindow();

	void node1Callback(const sensor_msgs::ImageConstPtr& msg);
	void node3Callback(const std_msgs::Int32::ConstPtr& _msgPow);

public slots:
	void hs_mH(int v);
	void hs_mS(int v);
	void hs_mV(int v);

	void hs_MH(int v);
	void hs_MS(int v);
	void hs_MV(int v);

private:
	Ui::MainWindow *ui;

	ros::NodeHandle nh;
	ros::Subscriber subPow;
	int state;
	
	image_transport::ImageTransport it;

	image_transport::Subscriber sub;

	image_transport::Publisher pub;
	sensor_msgs::ImagePtr msg;

	cv::Mat src, thres; 

	void perform();
	void publish();
	void display();
	void sliderSetup();

	int mh, ms, mv;
	int Mh, Ms, Mv;
};

#endif // MAINWINDOW_H
