#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv)
{
    Mat src = imread("test.jpg");

    Point2f src_vertices[4];
    src_vertices[0] = Point(270,120);
    src_vertices[1] = Point(395, 120);
    src_vertices[2] = Point(560, 480);
    src_vertices[3] = Point(110, 480);

    Point2f dst_vertices[4];
    dst_vertices[0] = Point(0, 0);
    dst_vertices[1] = Point(640, 0);
    dst_vertices[2] = Point(640, 480);
    dst_vertices[3] = Point(0, 480);

    Mat M = getPerspectiveTransform(src_vertices, dst_vertices);
    Mat dst(480, 640, CV_8UC3);
    warpPerspective(src, dst, M, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
}
