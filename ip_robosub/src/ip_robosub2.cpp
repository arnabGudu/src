#include <opencv2/opencv.hpp>

using namespace cv;

Mat initial_frame, hsv_frame, gray_frame, mask, canny, out_frame;
int mh = 0, ms = 116, mv = 68;
int MH = 24, MS = 255, MV = 134;

int size = 1, canny_thresh = 100;

void perform()
{
	cvtColor(initial_frame, hsv_frame, CV_BGR2HSV);
	inRange(hsv_frame, Scalar(mh, ms, mv), Scalar(MH, MS, MV), gray_frame);
	
	Mat element = getStructuringElement( MORPH_RECT, Size(2 * size + 1, 2 * size + 1), Point(size, size));
	 
	erode(gray_frame, mask, element);
	dilate(mask, mask, element);	

	Canny(mask, canny, canny_thresh, canny_thresh * 2, 3);

	std::vector<std::vector<Point> > contours;
	findContours( canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );

	std::vector<Rect> rect( contours.size() );
	cv::Rect min1;
	cv::Rect min2;
	
	for( size_t i = 0; i < contours.size(); i++ )
	{
		rect[i] = boundingRect( contours[i] );
		
		if (i == 0)
		{
			min1 = rect[i];
			min2 = rect[i + 1];
			continue;
		}
		
		if (rect[i].x < min1.x)
		{
			min2 = min1;
			min1 = rect[i];
		}
		else if (rect[i].x > min1.x && rect[i].x < min2.x)
		{
			min2 = rect[i];
		}
	}
	
	rectangle( initial_frame, min2.tl(), min1.br(), Scalar(0,0,255), 2 );	
	Point mid = (min2.tl() + min1.br()) / 2;
	std::cout<<"centre"<<mid<<std::endl;

	circle(initial_frame, mid, 4, Scalar(0, 0, 255), 2);
	
	//Mat drawing = Mat::zeros( canny.size(), CV_8UC3 );
	for( size_t i = 0; i < contours.size(); i++ )
	{
		rectangle( initial_frame, rect[i].tl(), rect[i].br(), Scalar(0,0,255), 2 );	//tl = topleft, br = buttomright
	}	

	//imshow( "Contours", drawing );
	
}

void show()
{
	perform();

	imshow("org", initial_frame);
	//imshow("thres", gray_frame);
	//imshow("erode", mask);
	//imshow("canny", canny);
}

void createWindow()
{
	namedWindow("thres", WINDOW_NORMAL);
	createTrackbar("mh", "thres", &mh, 255, NULL);
	createTrackbar("ms", "thres", &ms, 255, NULL);
	createTrackbar("mv", "thres", &mv, 255, NULL);
	createTrackbar("MH", "thres", &MH, 255, NULL);
	createTrackbar("MS", "thres", &MS, 255, NULL);
	createTrackbar("MV", "thres", &MV, 255, NULL);
	createTrackbar("size", "thres", &size, 100, NULL);
}

int main(int argc, char **argv)
{
	//createWindow();
	while(1)
	{
		initial_frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);
		
		show();
		if (waitKey(30) >= 0)
			break;
	}
}

