#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), nh(_nh)
{
	ui->setupUi(this);
	pub_key = nh.advertise<std_msgs::String>("key", 1);
	pub_pid = nh.advertise<tiburon_controller::pid_tuning>("pid", 10);

	defineArray();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				connect(slider[i][j][k], SIGNAL(valueChanged(int)), SLOT(Slider(void)));
			}
		}
		
		msg_pid.kp.push_back(0);
		msg_pid.ki.push_back(0);
		msg_pid.kd.push_back(0);
		msg_pid.sp.push_back(0);
	}
	connect(ui->reset, SIGNAL(pressed()), this, SLOT(load()));	
	connect(ui->save, SIGNAL(pressed()), this, SLOT(save()));	
	
	config_name = ros::package::getPath("tiburon_controller") + "/config/pid.conf";
	load();
}

MainWindow::~MainWindow() 
{
    delete ui;
}

/////////////////////////////////////////////////////////////
//////////////////////FILE FUNCTIONS/////////////////////////
/////////////////////////////////////////////////////////////

void MainWindow::load()
{
	ifstream config(config_name.c_str());
	for (int i = 0; i < 4; i++)
	{
		config>>msg_pid.kp[i];
		config>>msg_pid.ki[i];
		config>>msg_pid.kd[i];
		config>>msg_pid.sp[i];
	}
	config.close();
	load_slider();
	setLabels();
}

void MainWindow::save()
{
	ofstream config(config_name.c_str());
	for (int i = 0; i < 4; i++)
	{
		config<<msg_pid.kp[i]<<'\n';
		config<<msg_pid.ki[i]<<'\n';
		config<<msg_pid.kd[i]<<'\n';
		config<<msg_pid.sp[i]<<'\n';
	}
	config.close();
} 

/////////////////////////////////////////////////////////////
//////////////////////SLOT FUNCTIONS/////////////////////////
/////////////////////////////////////////////////////////////

void MainWindow::Slider(void)
{
	if (!isLoading)
	{
		for (int i = 0; i < 4; i++)
		{
			msg_pid.kp[i] = slider[i][0][0]->value() + (slider[i][0][1]->value() * 1.0) / 1000.0;			
			msg_pid.ki[i] = slider[i][1][0]->value() + (slider[i][1][1]->value() * 1.0) / 1000.0;			
			msg_pid.kd[i] = slider[i][2][0]->value() + (slider[i][2][1]->value() * 1.0) / 1000.0;
			msg_pid.sp[i] = slider[i][3][0]->value() * 10 + slider[i][3][1]->value();
		}
		setLabels();
		pub_pid.publish(msg_pid);
	}
}

/////////////////////////////////////////////////////////////
////////////////////PRIVATE FUNCTIONS////////////////////////
/////////////////////////////////////////////////////////////

void MainWindow::setLabels()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float val = (slider[i][j][0]->value() + (slider[i][j][1]->value() * 1.0) / 1000.0);
			value[i][j]->setText(QString::number(val));
		}
		value[i][3]->setText(QString::number(slider[i][3][0]->value() * 10 + slider[i][3][1]->value())); 
	}
}

void MainWindow::load_slider()
{
	isLoading = true;
	
	for(int i = 0; i < 4; i++)
	{
		slider[i][0][0]->setValue(int(msg_pid.kp[i]));
		slider[i][1][0]->setValue(int(msg_pid.ki[i]));
		slider[i][2][0]->setValue(int(msg_pid.kd[i]));
		slider[i][3][0]->setValue(msg_pid.sp[i] / 10);
		
		slider[i][0][1]->setValue((msg_pid.kp[i] - int(msg_pid.kp[i])) * 1000);
		slider[i][1][1]->setValue((msg_pid.ki[i] - int(msg_pid.ki[i])) * 1000);
		slider[i][2][1]->setValue((msg_pid.kd[i] - int(msg_pid.kd[i])) * 1000);
		slider[i][3][1]->setValue(msg_pid.sp[i] % 10);
	}
	isLoading = false;
}

/////////////////////////////////////////////////////////////
//////////////////////////KEY EVENTS/////////////////////////
/////////////////////////////////////////////////////////////

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	string str = "";
	if (event->key() == Qt::Key_W)
	{
		ui->teleop->setText("Key Pressed: W");
		str += 'W';
	}
	if (event->key() == Qt::Key_S)
	{
		ui->teleop->setText("Key Pressed: S");
		str += 'S';
	}
	if (event->key() == Qt::Key_A)
	{
		ui->teleop->setText("Key Pressed: A");
		str += 'A';
	}
	if (event->key() == Qt::Key_D)
	{
		ui->teleop->setText("Key Pressed: D");
		str += 'D';
	}
	if (event->key() == Qt::Key_Q)
	{
		ui->teleop->setText("Key Pressed: Q");
		str += 'Q';
	}
	if (event->key() == Qt::Key_E)
	{
		ui->teleop->setText("Key Pressed: E");
		str += 'E';
	}
	if (event->key() == Qt::Key_Z)
	{
		ui->teleop->setText("Key Pressed: Z");
		str += 'Z';
	}
	if (event->key() == Qt::Key_X)
	{
		ui->teleop->setText("Key Pressed: X");
		str += 'X';
	}

	std_msgs::String msg;
	msg.data = str;
	
	pub_key.publish(msg);
}
 
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	ui->teleop->setText("No Key Pressed");
	std_msgs::String msg;
	msg.data = "_";
	
	pub_key.publish(msg);
}
