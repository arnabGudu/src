#include "mainwindow.h"
#include <QApplication>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{		
	QApplication a(argc, argv);
	MainWindow mw;
	mw.show();
	float i = 0;
	while(TRUE)
	{
		a.processEvents();
		//mw.plot();
	}
	return 0;
}
