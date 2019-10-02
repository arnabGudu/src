#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::defineArray()
{
	slider[0][0][0] = ui->kp_heave_slider;
	slider[0][0][1] = ui->kp_heave_slider_2;
	slider[0][1][0] = ui->ki_heave_slider;
	slider[0][1][1] = ui->ki_heave_slider_2;
	slider[0][2][0] = ui->kd_heave_slider;
	slider[0][2][1] = ui->kd_heave_slider_2;
	slider[0][3][0] = ui->sp_heave_slider;
	slider[0][3][1] = ui->sp_heave_slider_2;
	
	slider[1][0][0] = ui->kp_roll_slider;
	slider[1][0][1] = ui->kp_roll_slider_2;
	slider[1][1][0] = ui->ki_roll_slider;
	slider[1][1][1] = ui->ki_roll_slider_2;
	slider[1][2][0] = ui->kd_roll_slider;
	slider[1][2][1] = ui->kd_roll_slider_2;
	slider[1][3][0] = ui->sp_roll_slider;
	slider[1][3][1] = ui->sp_roll_slider_2;
	
	slider[2][0][0] = ui->kp_pitch_slider;
	slider[2][0][1] = ui->kp_pitch_slider_2;
	slider[2][1][0] = ui->ki_pitch_slider;
	slider[2][1][1] = ui->ki_pitch_slider_2;
	slider[2][2][0] = ui->kd_pitch_slider;
	slider[2][2][1] = ui->kd_pitch_slider_2;
	slider[2][3][0] = ui->sp_pitch_slider;
	slider[2][3][1] = ui->sp_pitch_slider_2;
	
	slider[3][0][0] = ui->kp_yaw_slider;
	slider[3][0][1] = ui->kp_yaw_slider_2;
	slider[3][1][0] = ui->ki_yaw_slider;
	slider[3][1][1] = ui->ki_yaw_slider_2;
	slider[3][2][0] = ui->kd_yaw_slider;
	slider[3][2][1] = ui->kd_yaw_slider_2;
	slider[3][3][0] = ui->sp_yaw_slider;
	slider[3][3][1] = ui->sp_yaw_slider_2;	
	
	value[0][0] = ui->kp_heave_val;
	value[0][1] = ui->ki_heave_val;
	value[0][2] = ui->kd_heave_val;
	value[0][3] = ui->sp_heave_val;
	
	value[1][0] = ui->kp_roll_val;
	value[1][1] = ui->ki_roll_val;
	value[1][2] = ui->kd_roll_val;
	value[1][3] = ui->sp_roll_val;
	
	value[2][0] = ui->kp_pitch_val;
	value[2][1] = ui->ki_pitch_val;
	value[2][2] = ui->kd_pitch_val;
	value[2][3] = ui->sp_pitch_val;
	
	value[3][0] = ui->kp_yaw_val;
	value[3][1] = ui->ki_yaw_val;
	value[3][2] = ui->kd_yaw_val;
	value[3][3] = ui->sp_yaw_val;
}
