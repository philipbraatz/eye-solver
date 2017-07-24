#pragma once
#include <iostream>
#include <string>

#include <Windows.h>
#include <Winuser.h>

#include <vector>

#include "EvoNet.h"

using namespace std;
using namespace cv;

void StartMenu(RECT * area)
{
	double version = 0.4;
	cout << "Solution Finder " << version << ":" << endl;

	HWND handle;
	//string window;
	//cout << "Select name of window to watch: ";
	//cin >> window;
	Mat blank;
	namedWindow("Size Me", WINDOW_FREERATIO);
	//imshow("Size Me",blank);
	cout << "Set the size of the \"Size Me\" window and press enter";
	waitKey();

	if (handle = FindWindow(NULL, TEXT("Size Me"))) {
		//std::cout << "Found!"; 
		GetWindowRect(handle, area);
		DestroyWindow(handle);
	}
	//else {std::cout << "not found";}
}
////displays output of current generations best
//void CurrentGen(std::vector<EvoNet> fits,vector<float> input,)
//{
//	//LOGIC
//	for (size_t i = 0; i < fits.size(); i++)
//	{
//		fits[i].DoEpoch(input, false);
//		fits[i].repopulate(.5);
//		fits[i].updateStats(false);
//
//		//Data
//
//		fPoint p;
//		p.x = count;
//		p.y = fits[i].getBestScore();// /fits[i].getTime();
//		g.AddData(p, i);
//
//		std::cout << "\tScore: " << p.y << " | ";
//
//		sbest = fits[i].getBestOut();
//		for (size_t j = 0; j < answer.length(); j++)
//			std::cout << (char)(sbest[j] * (end - start) + start);
//	}
//
//	//PRINT
//	std::cout << std::endl;
//	std::cout << "Gen: " << count++ - 1 << " | ";
//}