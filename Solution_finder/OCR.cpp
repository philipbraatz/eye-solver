#include <string>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "utility.h"

using namespace cv;
using std::string;


//give it a name and a image and it will return the text
string textReconition(string Display, Mat ColorImage)
{
	Mat font = imread("C:\\Users\\Philip\\Documents\\Visual Studio 2015\\Projects\\OPENCVtest\\ascii.png", CV_LOAD_IMAGE_GRAYSCALE);

	namedWindow(Display, CV_WINDOW_KEEPRATIO);
	moveWindow(Display, 2050, 300);

	namedWindow("font", CV_WINDOW_KEEPRATIO);

	//image to read converted to binary
	Mat greytext(ColorImage.size(), CV_8U);
	cvtColor(ColorImage, greytext, COLOR_BGR2GRAY);
	Mat binarytext(greytext.size(), greytext.type());
	threshold(greytext, binarytext, 220, 230, THRESH_BINARY);//not in menu

	Mat ingame = binarytext;//(Range(500, 800), Range(1300, 1500));//(Range(30, 410), Range(2, 790));

	if (countNonZero(ingame) < 1)
	{//in menu
		threshold(greytext, binarytext, 60, 70, THRESH_BINARY);
	}
	//FONT READING -------------------------------------//
	threshold(font, font, 200, 230, THRESH_BINARY);//200,230

	if (font.empty()) //check whether the image is loaded or not
	{
		std::cout << "Error : Image cannot be loaded..!!" << std::endl;
		system("pause"); //wait for a key press
	}
	//resize font to make it the same character size
	int diff = 3;
	Mat newfont;
	Size size(font.cols*diff, font.rows*diff);
	resize(font, newfont, size);


	resizeWindow(Display, greytext.cols, greytext.rows);
	imshow(Display, binarytext);

	//displays font
	namedWindow("font", CV_WINDOW_KEEPRATIO);
	resizeWindow("font", font.cols, font.rows);
	imshow("font", font);


	return "";
}