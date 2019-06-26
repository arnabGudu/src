#include "face_recog.h"

using namespace cv;

int main(int argc, char** argv)
{
	VideoCapture cap(argv[1]);
	
	face f;	
	
	while (true)
	{
		cap >> f.frame;
		f.show();
	
		int k = waitKey(1);
		if (k == 'q')
			break;
	}	
}		
