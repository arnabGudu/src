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

#include <QSlider>
#include <QLabel>

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
    void Slider(void);
    void save();
    void load();

private:
    Ui::MainWindow *ui;
    ros::NodeHandle nh;
    ros::Publisher pub_pid, pub_key;
    tiburon_controller::pid_tuning msg_pid;
    string config_name;

    bool isLoading;
    QSlider * slider[4][4][2];
    QLabel * value[4][4];

	void defineArray();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void load_slider();
    void setLabels();
};

#endif // MAINWINDOW_H
