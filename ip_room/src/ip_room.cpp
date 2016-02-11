#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

Ptr<BackgroundSubtractor> backSub = createBackgroundSubtractorMOG2();

int main(int argc, char** argv)
{
	Mat src, mask;
	vector<vector<Point> > cnts;

	VideoCapture cap(1);
	
	while(cap.isOpened())
	{
		cap >> src;
		if (src.empty())
			break;
		resize(src, src, Size(512, 256));
		backSub->apply(src, mask);
		GaussianBlur(mask, mask, Size(5, 5), 0);
		
		erode(mask, mask, Mat(), Point(-1,-1), 4);
		dilate(mask, mask, Mat(), Point(-1,-1), 1);
        	findContours(mask, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		vector<Rect> boundRect( cnts.size() );
		
		for (int i = 0; i < cnts.size(); i++)
		{
			/*boundRect[i] = boundingRect(cnts[i]);
			if (boundRect[i].area() < 1000)
				continue;
			rectangle(src, boundRect[i], Scalar(0, 255, 0), 2);	
			//drawContours(src, cnts[i], Scalar(0,255,0), 2);			*/
		}

		imshow("src", src);
		imshow("mask", mask);
		
		waitKey(10);
	}
}
		
		

		

		
