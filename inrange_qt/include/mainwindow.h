#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ros/ros.h>
#include "opencv2/opencv.hpp"
#include "image_transport/image_transport.h"
#include "cv_bridge/cv_bridge.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(ros::NodeHandle _nh,QWidget *parent = 0);
	~MainWindow();
	void callback(const sensor_msgs::ImageConstPtr& msg);

private:
	Ui::MainWindow *ui;
	image_transport::ImageTransport it;
	image_transport::Subscriber sub;
	sensor_msgs::ImagePtr msg;
};

#endif // MAINWINDOW_H
