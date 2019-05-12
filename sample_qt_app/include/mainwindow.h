#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ros::NodeHandle _nh,QWidget *parent = 0);
    ~MainWindow();
    void callback(const std_msgs::Int32::ConstPtr& msg);

private:
    Ui::MainWindow *ui;
    ros::NodeHandle nh;
    ros::Subscriber sub;
};

#endif // MAINWINDOW_H
