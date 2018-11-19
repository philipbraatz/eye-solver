#pragma once
#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

//#include "../PEB.Utility/Utility.h"

using namespace cv;
using std::string;

const string ORDER = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

class OCR
{
public:

	void SetFont(string path, int width, int height);
	void OCR::SetVeiwer(string Display, Mat size);
	//give it a name and a color image and it will return the text using the set font
	string textReconition(string Display, Mat ColorImage);

private:
	std::vector<Mat> chars;
};



