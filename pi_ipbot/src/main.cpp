#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int L = 55, H = 255, a = 100, ht = 40, canny_thresh;

int main(int argc, char **argv)
{ 
	VideoCapture cap(1); 
	
	Mat src, gray, thresh, canny;
	
	namedWindow("thresh");
	namedWindow("src");
	createTrackbar("L", "thresh", &L, 255, NULL);
	createTrackbar("ht", "thresh", &ht, 255, NULL);
	createTrackbar("A", "thresh", &a, 255, NULL);
	createTrackbar("c", "thresh", &canny_thresh, 255, NULL);
	
	while(cap.isOpened())
	{
		cap >> src;
		
		resize(src, src, Size(), 0.25, 0.25, CV_INTER_AREA);
		
		Rect r(0, ht, src.cols, src.rows - ht);
		//rectangle(src, r, Scalar(255, 255, 0), 2);
		
		src = src(r);
		
		cvtColor(src, gray, CV_BGR2GRAY);
		blur(src, src, Size(1, 1));
		threshold(gray, thresh, L, 255, THRESH_BINARY_INV);

		dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
		erode(thresh, thresh, Mat(), Point(-1,-1), 2);
		
		vector< vector< Point > > contours;
		findContours(thresh, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

		int cnt_size = contours.size();
		
		vector <RotatedRect> min(cnt_size);
		float mid = 0, angle = 0;
		
		for (int i = 0; i < cnt_size; i++)
		{
			if (contourArea(contours[i]) < a)
				continue;
			
			min[i] = minAreaRect(contours[i]);
			
			Point2f rect_points[4];
			min[i].points(rect_points);
			for(int j = 0; j < 4; j++)
			{
				line(src, rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 2);
			}

			if (min[i].size.width < min[i].size.height)
			{
				angle += (int)min[i].angle;
			}
			else 
			{
				angle += (int)min[i].angle + 90;
			}
			mid += min[i].center.x;
			
			circle(src, Point(min[i].center.x, min[i].center.y), 2, Scalar(0,0,255), 2);
			drawContours(src, contours, i, Scalar(0,255,0), 2);
		}
		
		mid /= cnt_size;
		
		float error = src.cols/2 - mid;
		cout<<error<<endl;
		
		circle(src, Point(mid, src.rows/2), 2, Scalar(0,0,255), 2);
		line(src, Point(src.cols/2, 0), Point(src.cols/2, src.rows), Scalar(0, 0, 255), 2); 	

		imshow("thresh", thresh);
		imshow("src", src);
		waitKey(10);
	}
	return 0;
}
