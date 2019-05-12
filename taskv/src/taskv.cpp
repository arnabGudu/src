#include "opencv2/opencv.hpp"

using namespace cv;

Mat src, hsv, thres, mask, canny, blr;

int size = 5, canny_thresh = 100;
int mh = 6, ms = 167, mv = 76;
int Mh = 31, Ms = 255, Mv = 255;
int thr = 50, minH= 50, maxH = 10;

void trackBar()
{
	namedWindow("trackbar");
	createTrackbar("mHue", "trackbar", &mh, 255, NULL);
	createTrackbar("MHue", "trackbar", &Mh, 255, NULL);	
	createTrackbar("mSat", "trackbar", &ms, 255, NULL);
	createTrackbar("MSat", "trackbar", &Ms, 255, NULL);
	createTrackbar("mVal", "trackbar", &mv, 255, NULL);
        createTrackbar("MVal", "trackbar", &Mv, 255, NULL);
	
	namedWindow("erode");
	createTrackbar("size", "erode", &size, 21, NULL);
	createTrackbar("canny", "erode", &canny_thresh, 255, NULL);

	namedWindow("Houghline");
	createTrackbar("thres", "Houghline", &thr, 20, NULL);
	createTrackbar("minlinelen", "Houghline", &minH, 100, NULL);
	createTrackbar("maxlinegap", "Houghline", &maxH, 100, NULL);
}

void perform()
{
	cvtColor(src, hsv, CV_BGR2HSV);
	blur(hsv, blr, Size(15,15));

	inRange(blr, Scalar(mh,ms,mv), Scalar(Mh,Ms,Mv), thres);
	
	Mat element = getStructuringElement( MORPH_RECT, Size(2 * size + 1, 2 * size + 1), Point(size, size));
	 
	erode(thres, mask, element);
	dilate(mask, mask, element);

	Canny(mask, canny, canny_thresh, canny_thresh * 2, 3);
	
	std::vector<std::vector <Point> > contours;
	std::vector<Vec4i> hierarchy;
	
	findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
		drawContours(src, contours, i, Scalar(0,120,0), 2, 1, hierarchy, 0, Point());

	std::vector<Vec4i> lines;
	HoughLinesP(canny, lines, 1, CV_PI/180, thr, minH, maxH);

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		
		if ((l[0] - l[2]) > 0 || (l[0] - l[2]) < 0)
		{
			float angle = atan((l[3] - l[1]) / (l[2] - l[0])) * 180 / CV_PI;
			std::cout<<angle<<std::endl;
		}
	
	}
}
void show()
{
	imshow("original", src);
	//imshow("blur", blr);
	//imshow("hsv", hsv);
	imshow("trackbar", thres);
	imshow("erode", canny);
}

int main(int argc, char **argv)
{
	VideoCapture cap(argv[1]);
	
	trackBar();

	while(cap.isOpened())
	{
		cap >> src;		//like cin >> i, data goes from cap to src
		
		if (src.empty())
			break;
		
		int k = waitKey(10);	//10 ms delay
		if (k == ' ')
			while((k = waitKey(10)) != ' ')	//if space is presed frame stops 
			{
				if (k == 'q')
					return 0;
				perform();
				show();
			}			
		else if (k == 'q')
			return 0;	//if q is pressed, exit		
		
		perform();
		show();			//show image window
	}
	return 0;
}



