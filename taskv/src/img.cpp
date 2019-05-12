#include "opencv2/opencv.hpp"

using namespace cv;

Mat src, hsv, thres;

int blur_, erode_, dilate_;
int mh, ms, mv;
int Mh, Ms, Mv;

void trackBar()
{
        namedWindow("trackbar");
        //createTrackbar("blur", "trackbar", &blur_, 255, NULL);
        //createTrackbar("erode", "trackbar", &erode_, 10, NULL);
        //createTrackbar("dilate", "trackbar", &dilate_, 10, NULL);
        createTrackbar("mHue", "trackbar", &mh, 255, NULL);
        createTrackbar("MHue", "trackbar", &Mh, 255, NULL);
        createTrackbar("mSat", "trackbar", &ms, 255, NULL);
        createTrackbar("MSat", "trackbar", &Ms, 255, NULL);
        createTrackbar("mVal", "trackbar", &mv, 255, NULL);
        createTrackbar("MVal", "trackbar", &Mv, 255, NULL);
}

void perform()
{
        cvtColor(src, hsv, CV_BGR2HSV);
        inRange(hsv, Scalar(mh,ms,mv), Scalar(Mh,Ms,Mv), thres);
}
void show()
{
        imshow("original", src);
        //imshow("hsv", hsv);
        imshow("trackbar", thres);
}

int main(int argc, char **argv)
{
        src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
        trackBar();

	show();
	perform();
        int k = waitKey(0);       
	return 0;
}

