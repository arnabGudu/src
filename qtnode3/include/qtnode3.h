#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>

#include <ros/ros.h>
#include "image_transport/image_transport.h"
#include "opencv2/opencv.hpp"
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
		
	void node1Callback(const sensor_msgs::ImageConstPtr& msg1);
	void node2Callback(const sensor_msgs::ImageConstPtr& msg2);

public slots:
	void start();
	void stop();

private:
	Ui::MainWindow *ui;
	ros::NodeHandle nh;

	std_msgs::Int32 msg;
	ros::Publisher pub;

	image_transport::ImageTransport it;
	image_transport::Subscriber sub1;
	image_transport::Subscriber sub2;
};

#endif 
