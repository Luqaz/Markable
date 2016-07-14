#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\core\core.hpp>

#include <iostream>
#include <stdio.h>
#include <Windows.h>

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
vector<vector<Point>> contours;

vector<wstring> get_all_files_names_within_folder(string);
void Detect(string, string);

int main(int argc, const char** argv)
{
	auto files = get_all_files_names_within_folder("images");
	for each(auto file in files)
	{
		string filename(file.begin(), file.end());
		string filepath = "images/" + filename;

		Detect(filename, filepath);
		
		filename = "images/Marked_" + filename;
		imwrite(filename, source);
	}
	return 0;
}

vector<wstring> get_all_files_names_within_folder(string folder)
{
	vector<wstring> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	wstring stemp(search_path.begin(), search_path.end());
	HANDLE hFind = ::FindFirstFile(stemp.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

void Detect(string filename, string filepath)
{
	source = imread(filepath, CV_LOAD_IMAGE_ANYCOLOR);
	dst.create(source.size(), source.type());
	cvtColor(source, source_gray, CV_BGR2GRAY);

	blur(source_gray, detected_edges, Size(3, 3));
	Canny(detected_edges, detected_edges, 80, 80 * ratio, kernel_size);
	findContours(detected_edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	cout << "Processing " << filename << "...\t" << contours.size() << endl;

	int x_max = 0;
	int y_max = 0;
	int x_min = contours[0][0].x;
	int y_min = contours[0][0].y;
	for each (auto contour in contours)
	{
		for each (auto point in contour)
		{
			if (x_max < point.x)
				x_max = point.x;
			if (y_max < point.y)
				y_max = point.y;
			if (x_min > point.x)
				x_min = point.x;
			if (y_min > point.y)
				y_min = point.y;
		}
	}

	rectangle(source, Rect(x_min, y_min, x_max - x_min, y_max - y_min), Scalar(0, 255, 0), 2);
}