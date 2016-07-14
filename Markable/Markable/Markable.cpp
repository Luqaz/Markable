#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\core\core.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

string window_name = "Edges";
Mat source, source_gray;
Mat dst;
Mat detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
RNG rng(12345);

int main(int argc, const char** argv)
{
	source = imread("6.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	dst.create(source.size(), source.type());
	cvtColor(source, source_gray, CV_BGR2GRAY);
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	blur(source_gray, detected_edges, Size(3, 3));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Canny(detected_edges, detected_edges, 80, 80 * ratio, kernel_size);
	findContours(detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	int x_max = 0;
	int y_max = 0;
	int x_min = contours[0][0].x;
	int y_min = contours[0][0].y;
	for each (auto contour in contours)
	{
		for each (Point pnt in contour)
		{
			if (x_max < pnt.x)
				x_max = pnt.x;
			if (y_max < pnt.y)
				y_max = pnt.y;
			if (x_min > pnt.x)
				x_min = pnt.x;
			if (y_min > pnt.y)
				y_min = pnt.y;
		}
	}

	rectangle(source, Rect(x_min, y_min, x_max - x_min, y_max - y_min), Scalar(0, 255, 0), 2);
	imshow(window_name, source);

	while (true)
	{
		if(waitKey(20) == 0) break;
	}
	return 0;
}
