#pragma once
#include <iostream>
#include <string>

#include <Windows.h>
#include <Winuser.h>

#include <vector>

#include "EvoNet.h"

#include "utility.h"

using std::cout;
using namespace cv;

enum state
{
	NEW,
	CONTINUE,
	LOAD
};
void SizeWindow(RECT * area)
{
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

state StartMenu(RECT * area)
{
	double version = 0.5;
	cout << "Solution Finder " << version << ":" << endl;

	
	int chose =0;//SAFTY
	while (true)//LOAD / NEW
	{
		if (yesNoPromt("Would you like to load a Network?"))
		{//Load Network
			if (yesNoPromt("Do you want to train the Network?"))
			{//Train Network
				SizeWindow(area);
				return CONTINUE; 
			}
			else//Test Network
				return LOAD;
		}
		else//New Network
		{ 
			SizeWindow(area);
			return NEW;
		}
	}
}

