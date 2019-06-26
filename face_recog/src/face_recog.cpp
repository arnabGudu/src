#include "face_recog.h"

using namespace cv;
using namespace std;

face::face()
{
	String face_cascade_path = "/home/lucifer/catkin_ws/src/face_recog/haarcascade_frontalface_default.xml";

	String eyes_cascade_path = "/home/lucifer/catkin_ws/src/face_recog/haarcascade_eye.xml";

	if (! face_cascade.load(face_cascade_path))
		cout<<"face_cascade failed"<<endl;

	if (! eyes_cascade.load(eyes_cascade_path))
		cout<<"eyes_cascade failed"<<endl;
	
	scale = 3;
}

void face::show()
{
	perform();
	imshow("original", frame);
	imshow("gray", gray);
}

void face::perform()
{
	cvtColor(frame, gray, CV_BGR2GRAY);
	double fx = 1 / scale;
	resize( gray, gray, Size(), fx, fx, INTER_LINEAR_EXACT );
	equalizeHist( gray, gray );
	detect();
}

void face::detect()
{
	vector<Rect> faces;
	face_cascade.detectMultiScale(gray, faces);

	for (int i = 0; i < faces.size(); i++)
	{
		Rect f(	faces[i].x * scale, 
			faces[i].y * scale, 
			faces[i].width * scale, 
			faces[i].height * scale);
		
		rectangle(frame, f, Scalar(255,0,0), 2);
		Mat eye = gray(faces[i]);
		
		vector<Rect> eyes;
		eyes_cascade.detectMultiScale(eye, eyes);
		
		for (int j = 0; j < eyes.size(); j++)
		{
			Rect e((faces[i].x + eyes[j].x) * scale, 
			       (faces[i].y + eyes[j].y) * scale, 
				eyes[j].width * scale, 
				eyes[j].height * scale);
			
			rectangle(frame, e, Scalar(0,0,255), 2);
		}
	} 		
}
