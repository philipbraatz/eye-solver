#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>

using std::string;

//MAKE ALL FUNCTIONS
//		static
//!!!!!!!!!!!!!!!!!!

//int x, double y
struct ifPoint
{
	int x;
	double y;
};
//double x. y
struct fPoint
{
	double x, y;
};

//static double RandNum() { return static_cast <double> (rand()) / static_cast <double> (RAND_MAX); }

//static double GetTotalDif(std::vector<double> input, std::vector<double> output)
//{
//	double total = 0;
//	for (auto i = 0; i < input.size(); i++)
//		total += abs(input[i] - output[i]);
//	return total;
//}

//Yes true
//No false
static bool yesNoPromt(string question)
{
	char ans;
	std::cout << question << " (y/n)" << std::endl;
	while (true)
	{
		std::cin >> ans;
		switch (ans)
		{
		case 'y':
			return true;
			break;
		case 'n':
			return false;
			break;
		default:
			break;
		}
	}
}

//string GetActiveWindowTitle()
//{
//	char wnd_title[256];
//	HWND hwnd = GetForegroundWindow(); // get handle of currently active window
//	GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
//	return wnd_title;
//}