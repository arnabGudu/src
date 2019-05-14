#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), it(_nh)
{
    ui->setupUi(this);
    sub = it.subscribe("camera/image1", 1, &MainWindow::callback, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::callback(const sensor_msgs::ImageConstPtr& msg)
{
	cv::Mat src = cv_bridge::toCvShare(msg, "bgr8")->image;
	cv::Mat thres = src;
		
	cv::inRange(src, cv::Scalar(125,125,125), cv::Scalar(255,255,255), thres);
	
	cv::cvtColor(thres, thres, CV_GRAY2BGR);
	cv::cvtColor(thres, thres, CV_BGR2RGB);
	QPixmap p = QPixmap::fromImage(QImage((unsigned char*) thres.data, thres.cols, thres.rows, thres.step, QImage::Format_RGB888));
	//p = p.scaledToWidth(500);
	
	ui->label_node->setPixmap(p);

}
