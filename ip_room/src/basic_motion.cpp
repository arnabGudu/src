#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

int er = 1, di = 1, Min = 25;

Mat frame, firstFrame; 
 
void perform()
{
	Mat gray, frameDelta, thresh;
	vector<vector<Point> > cnts;

    	cvtColor(frame, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(21, 21), 0);

	absdiff(firstFrame, gray, frameDelta);
	threshold(frameDelta, thresh, Min, 255, THRESH_BINARY);

	dilate(thresh, thresh, Mat(), Point(-1,-1), di);
	erode(thresh, thresh, Mat(), Point(-1,-1), er);

	findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<Point> contours;

	vector<Rect> boundRect( cnts.size() );

	//Point tl = frame.size(), br = Point(0,0);

	for (int i = 0; i < cnts.size(); i++)
	{
		boundRect[i] = boundingRect(cnts[i]);
		if (boundRect[i].area() < 1000)
			continue;

		/*if (boundRect[i].tl().x < tl.x)
			tl = boundRect[i].tl();
		if (boundRect[i].br().y > br.y)
			br = boundRect[i].br();*/

		rectangle(frame, boundRect[i], Scalar(0, 255, 0), 2);
		contours.insert(contours.end(), cnts[i].begin(), cnts[i].end());	
		rectangle(frame, boundingRect(contours), Scalar(255, 0, 0), 2);
		
	}
	imshow("Camera", frame);
	imshow("thresh", thresh);
	//imshow("frameDelta", frameDelta);
}


int main(int argc, char **argv) {
    
    VideoCapture camera(argv[1]);
    
    namedWindow("c");
    createTrackbar("E", "c", &er, 10, NULL);
    createTrackbar("D", "c", &di, 10, NULL);
    createTrackbar("min", "c", &Min, 255, NULL);

    //camera.set(3, 512);
    //camera.set(4, 288);

    sleep(3);
    camera.read(frame);
    
    //resize(frame, frame, Size(frame.rows/2, frame.cols/2));

    cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
    GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);

    while(camera.read(frame)) 
    {
        int k = waitKey(10);
	if (k == ' ')
		while((k = waitKey(10)) != ' ')
		{
			if (k == 'q')
				return 0;
			perform();
		}			
	else if (k == 'q')
		return 0;
	perform();  
    }  
}
