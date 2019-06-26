#ifndef FACE
#define FACE

#include "ros/ros.h"
#include "opencv2/opencv.hpp"

using namespace cv;

class face
{

public:
	face();
	void show();
	Mat frame;

private:
	Mat gray;
	void perform();
	void detect();

	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;

	float scale;
};

#endif
