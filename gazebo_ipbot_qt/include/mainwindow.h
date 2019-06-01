#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ros/ros.h>
#include "opencv2/opencv.hpp"
#include "image_transport/image_transport.h"
#include "cv_bridge/cv_bridge.h"
#include <gazebo_ipbot_qt/pid.h>
#include <gazebo_ipbot_qt/tuning.h>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ros::NodeHandle _nh,QWidget *parent = 0);
    ~MainWindow();
    void callback(const sensor_msgs::ImageConstPtr& _msg);

public slots:
    	void hs_lkp(int v);
    	void hs_lkd(int v);
    	void hs_lki(int v);

    	void hs_akp(int v);
    	void hs_akd(int v);
    	void hs_aki(int v);

      void vs_vel(int v);

private:
    Ui::MainWindow *ui;
    ros::NodeHandle nh;

    ros::Publisher pubPid;
    ros::Publisher pubTuning;

    image_transport::ImageTransport it;
  	image_transport::Subscriber sub;
  	Mat src, blr, hsv, canny, thres;

    void sliderSetup();
    void Publish();
    void perform();

    int lkp, lkd, lki;
    int akp, akd, aki;
    int vel;
};

#endif // MAINWINDOW_H
