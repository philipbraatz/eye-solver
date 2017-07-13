#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <Windows.h>

using std::string;

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

//string GetActiveWindowTitle()
//{
//	char wnd_title[256];
//	HWND hwnd = GetForegroundWindow(); // get handle of currently active window
//	GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
//	return wnd_title;
//}