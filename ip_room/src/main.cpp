#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int size = 10;
int ht = 0;

int main(int argc, char** argv)
{
	VideoCapture cap(argv[1]);

	Mat src, gray;
	vector< Rect >loc;
	vector< double >weights;
	
	HOGDescriptor hog;
	hog.setSVMDetector(hog.getDefaultPeopleDetector());

	namedWindow("Video");
	createTrackbar("size", "Video", &size, 100, NULL);
	createTrackbar("ht", "Video", &ht, 200, NULL);

	while(cap.isOpened())
	{
		cap >> src;
		
		if (src.empty())
			break;
		
		//resize(src, src, Size(512, 256));
		resize(src, src, Size(), (float)size/10, (float)size/10, INTER_LINEAR_EXACT );
		cvtColor(src, gray, CV_BGR2GRAY);

		hog.detectMultiScale(gray, loc, weights);

		for (int i = 0; i < loc.size(); i++)
		{
			//if (loc[i].height < (float)ht)
				//continue;
			if (weights[i] < 0.8)
				continue;

			Rect r( loc[i].x, 
				loc[i].y, 
				loc[i].width, 
				loc[i].height);
			
			cout<<weights[i]<<endl;
		
			rectangle(src, r, Scalar(0,255,0), 2);
		}

		imshow("Video", src);
		waitKey(60);
	}
}
		
		
