#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

struct Screen
{
	Mat image;
	char* name;
	int width;
	int height;
	int x;
	int y;
};