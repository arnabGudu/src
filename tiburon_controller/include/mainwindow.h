#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/String.h>
#include <iostream>
#include <QKeyEvent>
#include "tiburon_controller/pid_tuning.h"

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
    tiburon_controller::pid_tuning msg_pid;
    string config_name;
	
    int index, kp[4], kd[4], ki[4];
    bool index_changed_flag;

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void setLabels();
    void publish_pid();
    void load();
    void print();
};

#endif // MAINWINDOW_H
