#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool find_dist(vector<Point> cnt1, vector<Point> cnt2)
{
	Moments mu1, mu2;
	Point2f mc1, mc2;
	
	mu1 = moments(cnt1);
	mu2 = moments(cnt2);
	
	mc1 = Point2f( mu1.m10/mu1.m00, mu1.m01/mu1.m00 );
	mc2 = Point2f( mu2.m10/mu2.m00, mu2.m01/mu2.m00 );
	
	float dist = sqrt(pow(mc1.x - mc2.x, 2) + pow(mc1.y - mc2.y, 2));
	if (dist < 150)
		return true;
	else
		return false;	
}

int main(int argc, char** argv)
{
	Mat src, gray, thresh;

	src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	cvtColor(src, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, gray, Size(21, 21), 0);

	threshold(gray, thresh, 25, 255, THRESH_BINARY);
	dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
	erode(thresh, thresh, Mat(), Point(-1,-1), 2);

	vector<vector<Point> > cnts;
	findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		
	int length = cnts.size();
	int status[length];

	cout<<"length: "<<length<<endl;

	for (int i = 0; i < length; i++)
		status[i] = 0;
	
	for (int i = 0; i < length; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			if (find_dist(cnts[i], cnts[j]))
			{
				int val = min(status[i], status[j]);
				status[i] = val;
				status[j] = status[i];
			}
			else
			{
				if (status[j] == status[i])
					status[j] = i + 1;
			}
		}
		cout<<status[i]<<"\t";
	}
	cout<<endl;
	int max = 0;
	for (int i = 0; i < length; i++)
		if (status[i] > max)
			max = status[i];
	max++;
	cout<<"max"<<max<<endl;

	Rect boundRect[max];
	vector<vector<Point> > contours(max);
	vector<Point> hull;

	cvtColor(thresh, src, COLOR_GRAY2BGR);
	
	for (int i = 0; i < max; i++)
	{
		for (int j = 0; j < length; j++)
		{	
			cout<<status[j]<<"\t"<<i;
			if (status[j] == i)
			{
				contours[i].insert(contours[i].end(), cnts[j].begin(), cnts[j].end());	
				cout<<"\tmatch";
				
			}
			cout<<endl;
			drawContours(src, cnts, j, Scalar(125,0, 50+100*j), 2, 1);
		}	
		cout<<"draw"<<i<<endl;				
		boundRect[i] = boundingRect(contours[i]);
		rectangle(src, boundRect[i], Scalar(255, 100 * i, 0), 2);
	}
		
	imshow("src", src);
	//imshow("gray", gray);
	//imshow("thresh", thresh);
	waitKey(0);
}
