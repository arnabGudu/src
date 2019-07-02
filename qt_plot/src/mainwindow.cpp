#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), nh(_nh)
{
    ui->setupUi(this);
    sub = nh.subscribe("numbers", 1, &MainWindow::callback, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::callback(const std_msgs::Int32::ConstPtr& msg)
{
    ui->lcd->display(msg->data);
}
