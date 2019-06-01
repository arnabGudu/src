#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), it(_nh)
{
    ui->setupUi(this);
    pubPid = nh.advertise<gazebo_ipbot_qt::pid>("pid", 1000);
  	pubTuning = nh.advertise<gazebo_ipbot_qt::tuning>("tuning", 1000);
    sub = it.subscribe("front_camera/image_raw", 1, &MainWindow::callback, this);

    sliderSetup();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::callback(const sensor_msgs::ImageConstPtr& _msg)
{
  src = cv_bridge::toCvShare(_msg, "bgr8")->image;
  perform();

  cv::cvtColor(src, src, CV_BGR2RGB);
  QPixmap p = QPixmap::fromImage(QImage((unsigned char*) src.data, src.cols, src.rows, src.step, QImage::Format_RGB888));
  p = p.scaledToWidth(500);

  ui->label_img->setPixmap(p);
}

void MainWindow::perform()
{
  int ht = 285, wd = 70;
  int h = 0, s = 0, v = 0;
  int H = 255, S = 255, V = 80;

  Mat hsv, blr, canny, thres;

  blur(src, blr, Size(10, 10));
	cvtColor(src, hsv, CV_BGR2HSV);

	inRange(hsv, Scalar(h, s, v), Scalar(H, S, V), thres);

	Canny(thres, canny, 100, 200, 3);

	Rect r(120,ht,407,wd);

	vector< vector< Point > > contours;
	findContours(canny(r), contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vector <RotatedRect> min(contours.size());
	int avgX = 0;
	int angle = 0;

	//find minAreaRect
	for (int i = 0; i < contours.size(); i++)
	{
		min[i] = minAreaRect(contours[i]);
	}

	//draw minAreaRect
	for (int i = 0; i < contours.size(); i++)
	{
		Point2f rect_points[4];
		min[i].points(rect_points);
		for(int j = 0; j < 4; j++)
		{
			line(src(r), rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 2);
		}

		cout<<"cX:"<<(int)min[i].center.x + 120;
		cout<<" cY:"<<(int)min[i].center.y + ht;
		//cout<<" angle:"<<(int)min[i].angle<<endl;

		if (min[i].size.width < min[i].size.height)
		{
			angle += (int)min[i].angle;
			cout<<"angle1:"<<min[i].angle<<endl;
		}
		else
		{
			angle += (int)min[i].angle + 90;
			cout<<"angle2:"<<min[i].angle + 90<<endl;
		}
		avgX += (int)min[i].center.x;
	}

	rectangle(src, r, Scalar(0,0,255), 2);
	line(src, Point(323,0), Point(323, 487), Scalar(0, 255, 0), 2);
	//circle(src, Point(avgX, 200), 4, Scalar(255,0,0), 2);

  gazebo_ipbot_qt::pid msg;

	if (contours.size() > 0)
	{
		msg.linear.data = avgX / contours.size() + 120 - 323;
		msg.angular.data = angle / contours.size();
	}
	else
	{
		msg.linear.data = 0;
		msg.angular.data = 0;
	}

  pubPid.publish(msg);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void MainWindow::Publish()
{
  gazebo_ipbot_qt::tuning msg;

  msg.lkp.data = lkp;
  msg.lkd.data = lkd;
  msg.lki.data = lki;

  msg.akp.data = lkp;
  msg.akd.data = lkd;
  msg.aki.data = lki;

  msg.vel.data = vel;

  pubTuning.publish(msg);
}

void MainWindow::sliderSetup()
{
	lkp = 0;
	lkd = 0;
	lki = 0;

	akp = 0;
	akd = 0;
	aki = 0;

  vel = 0;

	ui->hs_lkp->setValue(lkp);
	ui->hs_lkd->setValue(lkd);
	ui->hs_lki->setValue(lki);

	ui->hs_akp->setValue(akp);
	ui->hs_akd->setValue(akd);
	ui->hs_aki->setValue(aki);

	connect(ui->hs_lkp, SIGNAL(valueChanged(int)), this, SLOT(hs_lkp(int)));
	connect(ui->hs_lkd, SIGNAL(valueChanged(int)), this, SLOT(hs_lkd(int)));
	connect(ui->hs_lki, SIGNAL(valueChanged(int)), this, SLOT(hs_lki(int)));

	connect(ui->hs_akp, SIGNAL(valueChanged(int)), this, SLOT(hs_akp(int)));
	connect(ui->hs_akd, SIGNAL(valueChanged(int)), this, SLOT(hs_akd(int)));
	connect(ui->hs_aki, SIGNAL(valueChanged(int)), this, SLOT(hs_aki(int)));

  connect(ui->vs_vel, SIGNAL(valueChanged(int)), this, SLOT(vs_vel(int)));
}
void MainWindow::hs_lkp(int v)
{
	lkp = v;
  Publish();
}
void MainWindow::hs_lkd(int v)
{
	lkd = v;
  Publish();
}
void MainWindow::hs_lki(int v)
{
	lki = v;
  Publish();
}
void MainWindow::hs_akp(int v)
{
	akp = v;
  Publish();
}
void MainWindow::hs_akd(int v)
{
	akd = v;
  Publish();
}
void MainWindow::hs_aki(int v)
{
	aki = v;
  Publish();
}
void MainWindow::vs_vel(int v)
{
	vel = v;
  Publish();
}
