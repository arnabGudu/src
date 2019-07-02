#include "mainwindow.h"
#include <QApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{		
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;
    QApplication app(argc, argv);
    MainWindow mw(nh);
    mw.show();
    ros::Rate rate(10);
    while(ros::ok())
    {
        ros::spinOnce();
        rate.sleep();
        app.processEvents();
    }
    return 0;
}
