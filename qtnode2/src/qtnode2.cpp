#include "qtnode2.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), it(_nh)
{
	ui->setupUi(this);
	sub = it.subscribe("camera/image1", 1, &MainWindow::node1Callback, this);
	pub = it.advertise("camera/image2", 1);	
	
	sliderSetup();	
}


void MainWindow::sliderSetup()
{
	ui->hs_mh->setValue(mh);
	ui->hs_ms->setValue(ms);
	ui->hs_mv->setValue(mv);

	ui->hs_Mh->setValue(Mh);
	ui->hs_Ms->setValue(Ms);
	ui->hs_Mv->setValue(Mv);

	connect(ui->hs_mh, SIGNAL(valueChanged(int)), this, SLOT(hs_mH(int)));
	connect(ui->hs_ms, SIGNAL(valueChanged(int)), this, SLOT(hs_mS(int)));
	connect(ui->hs_mv, SIGNAL(valueChanged(int)), this, SLOT(hs_mV(int)));
	
	connect(ui->hs_Mh, SIGNAL(valueChanged(int)), this, SLOT(hs_MH(int)));
	connect(ui->hs_Ms, SIGNAL(valueChanged(int)), this, SLOT(hs_MS(int)));
	connect(ui->hs_Mv, SIGNAL(valueChanged(int)), this, SLOT(hs_MV(int)));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::node1Callback(const sensor_msgs::ImageConstPtr& msg1)
{
	src = cv_bridge::toCvShare(msg1, "bgr8")->image;
	
	QPixmap p = QPixmap::fromImage(QImage((unsigned char*) src.data, src.cols, src.rows, QImage::Format_RGB888));
	p = p.scaledToWidth(500);
	
	ui->label_node1->setPixmap(p);

	display();
	publish();
}

void MainWindow::publish()
{
	msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", thres).toImageMsg();
	pub.publish(msg);
}

void MainWindow::perform()
{
	cv::Mat hsv, blr;

	cv::cvtColor(src, hsv, CV_BGR2HSV);
	thres = hsv;
	//cv::blur(hsv, blr, cv::Size(15,15));
	cv::inRange(hsv, cv::Scalar(mh,ms,mv), cv::Scalar(Mh,Ms,Mv), thres);
}

void MainWindow::display()
{
	perform();

	QPixmap p = QPixmap::fromImage(QImage((unsigned char*) thres.data, thres.cols, thres.rows, QImage::Format_MonoLSB));
	//p = p.scaledToWidth(500);
	ui->label_node2->setPixmap(p);
} 

void MainWindow::hs_mH(int v)
{
	mh = v;
	std::cout<<"mh"<<v<<std::endl;
}

void MainWindow::hs_mS(int v)
{
	ms = v;
	std::cout<<"ms"<<v<<std::endl;
}

void MainWindow::hs_mV(int v)
{
	mv = v;
	std::cout<<"mv"<<v<<std::endl;
}

void MainWindow::hs_MH(int v)
{
	Mh = v;
	std::cout<<"Mh"<<v<<std::endl;
}

void MainWindow::hs_MS(int v)
{
	Ms = v;
	std::cout<<"Ms"<<v<<std::endl;
}

void MainWindow::hs_MV(int v)
{
	Mv = v;
	std::cout<<"Mv"<<v<<std::endl;
}


