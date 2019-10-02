#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/String.h>
#include <iostream>
#include <QKeyEvent>
#include "pi_ipbot/pid_pi.h"

#define max_index 2

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ros::NodeHandle _nh,QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slider(void);
    void save();
    void index_changed(void);

private:
    Ui::MainWindow *ui;
    ros::NodeHandle nh;
    ros::Publisher pub_pid, pub_key;
    pi_ipbot::pid_pi msg_pid;
    string config_name;
	
    int index, kp[max_index], kd[max_index], ki[max_index], vel;
    bool index_changed_flag;

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void setLabels();
    void publish_pid();
    void load();
    void print();
};

#endif // MAINWINDOW_H
