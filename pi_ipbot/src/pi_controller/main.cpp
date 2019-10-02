#include "pi_controller/mainwindow.h"
#include <QApplication>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{		
    ros::init(argc, argv, "tiburon_controller");
    ros::NodeHandle nh;
    QApplication a(argc, argv);
    MainWindow w(nh);
    //w.adjustSize();
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
