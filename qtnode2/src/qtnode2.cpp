#include "qtnode2.h"
#include "ui_mainwindow.h"

cv::Mat src1;

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), nh(_nh), it(_nh)
{
	ui->setupUi(this);
	state = 1;
	subPow = nh.subscribe("power", 1000, &MainWindow::node3Callback, this); 
	sub = it.subscribe("camera/image1", 1, &MainWindow::node1Callback, this);
	pub = it.advertise("camera/image2", 1);	
	
	sliderSetup();	
}


void MainWindow::sliderSetup()
{
	mh = 0;
	ms = 0;
	mv = 0;
	
	Mh = 255;
	Ms = 255;
	Mv = 255;

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


void MainWindow::node3Callback(const std_msgs::Int32::ConstPtr& _msgPow)
{
	state = _msgPow->data;
}

void MainWindow::node1Callback(const sensor_msgs::ImageConstPtr& msg1)
{
	if (state)
	{
		src = cv_bridge::toCvShare(msg1, "bgr8")->image;
		perform();

		cv::cvtColor(src, src, CV_BGR2RGB);
		QPixmap p = QPixmap::fromImage(QImage((unsigned char*) src.data, src.cols, src.rows, src.step, QImage::Format_RGB888));
		p = p.scaledToWidth(500);
	
		ui->label_node1->setPixmap(p);

		display();
	}
	else
	{
		ui->label_node1->setText("No image from node 1");
		ui->label_node2->setText("No image for threshold");
	}
}

void MainWindow::publish()
{
	msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", thres).toImageMsg();
	pub.publish(msg);
}

void MainWindow::perform()
{
	cv::Mat hsv, blr;
	cv::cvtColor(src, hsv, CV_BGR2HSV);
	cv::blur(hsv, blr, cv::Size(15,15));

	cv::inRange(blr, cv::Scalar(mh,ms,mv), cv::Scalar(Mh,Ms,Mv), thres);	
	cv::cvtColor(thres, thres, CV_GRAY2BGR);
	
	publish();	

	cv::cvtColor(thres, thres, CV_BGR2RGB);
}

void MainWindow::display()
{
	QPixmap p = QPixmap::fromImage(QImage((unsigned char*) thres.data, thres.cols, thres.rows, thres.step, QImage::Format_RGB888));
	p = p.scaledToWidth(500);
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


