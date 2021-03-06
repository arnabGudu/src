#include "qtnode2.h"
#include <QApplication>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{		
	ros::init(argc, argv, "node2");
	ros::NodeHandle nh;
	QApplication a(argc, argv);
	MainWindow w(nh);
	w.show();
	ros::Rate rate(10);
	
	while(ros::ok())
	{
		ros::spinOnce();
		rate.sleep();
		a.processEvents();
	}
	return 0;
}
