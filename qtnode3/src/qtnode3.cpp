#include "qtnode3.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), nh(_nh), it(_nh)
{
	ui->setupUi(this);

	pub = nh.advertise<std_msgs::Int32>("power", 1000);
	sub1 = it.subscribe("camera/image1", 1, &MainWindow::node1Callback, this);
	sub2 = it.subscribe("camera/image2", 1, &MainWindow::node2Callback, this);

	connect(ui->b_start,SIGNAL(pressed()),this,SLOT(start()));
	connect(ui->b_stop,SIGNAL(pressed()),this,SLOT(stop()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::node1Callback(const sensor_msgs::ImageConstPtr& msg1)
{
	if (msg.data == 1)
	{
	cv::Mat node1 = cv_bridge::toCvShare(msg1, "bgr8")->image;
	cv::cvtColor(node1, node1, CV_BGR2RGB);

	QPixmap p = QPixmap::fromImage(QImage((unsigned char*) node1.data, node1.cols, node1.rows, QImage::Format_RGB888));

	p = p.scaledToWidth(500);
	ui->label_node1->setPixmap(p);
	}

	else
	{
		ui->label_node1->setText("No image from node 1");
	}
}

void MainWindow::node2Callback(const sensor_msgs::ImageConstPtr& msg2)
{
	if (msg.data == 1)
	{
	cv::Mat node2 = cv_bridge::toCvShare(msg2, "bgr8")->image;
	cv::cvtColor(node2, node2, CV_BGR2RGB);

	QPixmap p = QPixmap::fromImage(QImage((unsigned char*) node2.data, node2.cols, node2.rows, QImage::Format_RGB888));

	p = p.scaledToWidth(500);
	ui->label_node2->setPixmap(p);
	}

	else
	{
		ui->label_node2->setText("No image from node 2");
	}
}

void MainWindow::start()
{	
	msg.data = 1;
	pub.publish(msg);
	std::cout<<"start"<<std::endl;
}

void MainWindow::stop()
{
	msg.data = 0;
	pub.publish(msg);
	std::cout<<"stop"<<std::endl;
}
