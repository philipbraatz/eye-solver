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

struct fPoint
{
	float x, y;
};

static float RandNum() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }

static float GetTotalDif(std::vector<float> input, std::vector<float> output)
{
	float total = 0;
	for (size_t i = 0; i < input.size(); i++)
		total += abs(input[i] - output[i]);
	return total;
}

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