#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ros::NodeHandle _nh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), nh(_nh)
{
	ui->setupUi(this);
	pub_key = nh.advertise<std_msgs::String>("key", 1);
	pub_pid = nh.advertise<tiburon_controller::pid_tuning>("pid", 10);

	ui->comboBox->insertItem(0, "ROLL");
	ui->comboBox->insertItem(1, "PITCH");
	ui->comboBox->insertItem(2, "YAW");
	ui->comboBox->insertItem(3, "DEPTH"); 	

	connect(ui->hs_kp, SIGNAL(valueChanged(int)), SLOT(slider(void)));
    connect(ui->hs_ki, SIGNAL(valueChanged(int)), SLOT(slider(void)));
    connect(ui->hs_kd, SIGNAL(valueChanged(int)), SLOT(slider(void)));
	connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(index_changed(void)));
	connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(save()));	
	
	index = 0;
	index_changed_flag = false;
	for (int i = 0; i < 4; i++)
	{
		kp[i] = 0;
		kd[i] = 0;
		ki[i] = 0;
	}
	config_name = ros::package::getPath("tiburon_controller") + "/config/pid.conf";
	
	load();
	for(index = 0; index < 4; index++)
	{ 
		publish_pid();
	}
	index = 0;
}

void MainWindow::print()
{
	for (int i = 0; i < 4; i++)
		cout<<kp[i]<<'\t'<<kd[i]<<'\t'<<ki[i]<<endl;
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
		config>>kp[i];
		config>>kd[i];
		config>>ki[i];
	}
	config.close();
	print();
	index_changed();
}

void MainWindow::save()
{
	ofstream config(config_name.c_str());
	for (int i = 0; i < 4; i++)
	{
		config<<kp[i]<<'\n';
		config<<kd[i]<<'\n';
		config<<ki[i]<<'\n';
	}
	config.close();
	print();
} 

/////////////////////////////////////////////////////////////
//////////////////////SLOT FUNCTIONS/////////////////////////
/////////////////////////////////////////////////////////////

void MainWindow::slider(void)
{
	if (!index_changed_flag)
	{
		kp[index] = ui->hs_kp->value();
		kd[index] = ui->hs_kd->value();
		ki[index] = ui->hs_ki->value();
	
		setLabels();
		publish_pid();
	}	
}

void MainWindow::index_changed(void)
{
	index_changed_flag = true;
	
	index = ui->comboBox->currentIndex();
	ui->hs_kp->setValue(kp[index]);
	ui->hs_kd->setValue(kd[index]);
	ui->hs_ki->setValue(ki[index]);
	
	index_changed_flag = false;
	
	setLabels();
	publish_pid();
}

/////////////////////////////////////////////////////////////
////////////////////PRIVATE FUNCTIONS////////////////////////
/////////////////////////////////////////////////////////////

void MainWindow::setLabels()
{
	ui->vkp->setText(QString::number(ui->hs_kp->value() * 1.0 / 100));
    ui->vkd->setText(QString::number(ui->hs_kd->value() * 1.0 / 100));
    ui->vki->setText(QString::number(ui->hs_ki->value() * 1.0 / 100));
}

void MainWindow::publish_pid()
{
	msg_pid.kp = kp[index];
    msg_pid.kd = kd[index];
    msg_pid.ki = ki[index];
	msg_pid.mode = index;
	pub_pid.publish(msg_pid);
}

/////////////////////////////////////////////////////////////
//////////////////////////KEY EVENTS/////////////////////////
/////////////////////////////////////////////////////////////

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	string str = "";
	if (event->key() == Qt::Key_W)
	{
		ui->myLabel->setText("Key Pressed: W");
		str += 'W';
	}
	if (event->key() == Qt::Key_S)
	{
		ui->myLabel->setText("Key Pressed: S");
		str += 'S';
	}
	if (event->key() == Qt::Key_A)
	{
		ui->myLabel->setText("Key Pressed: A");
		str += 'A';
	}
	if (event->key() == Qt::Key_D)
	{
		ui->myLabel->setText("Key Pressed: D");
		str += 'D';
	}
	if (event->key() == Qt::Key_Q)
	{
		ui->myLabel->setText("Key Pressed: Q");
		str += 'Q';
	}
	if (event->key() == Qt::Key_E)
	{
		ui->myLabel->setText("Key Pressed: E");
		str += 'E';
	}
	if (event->key() == Qt::Key_Z)
	{
		ui->myLabel->setText("Key Pressed: Z");
		str += 'Z';
	}
	if (event->key() == Qt::Key_X)
	{
		ui->myLabel->setText("Key Pressed: X");
		str += 'X';
	}

	std_msgs::String msg;
	msg.data = str;
	
	pub_key.publish(msg);
}
 
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	ui->myLabel->setText("No Key Pressed");
	std_msgs::String msg;
	msg.data = "_";
	
	pub_key.publish(msg);
}
