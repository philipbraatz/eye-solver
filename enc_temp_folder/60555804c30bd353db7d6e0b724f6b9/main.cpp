#pragma once
#include <ctime>

#include <iomanip>

#include <string>
#include <iostream>
#include <conio.h>

#include <vector>
#include <algorithm>

#include <Windows.h>
#include <Winuser.h>

#include "graphic.h"

#include "EvoNet.h"
#include "Nnet.h"
#include "utility.h"
#include "capture.h"
#include "Menu.h"
#include "OCR.h"

using std::vector;
using namespace cv;

int main()
{
	//pre-initialization

	///hide console
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	srand(static_cast <unsigned> (time(0)));

	//start menu
	RECT* pArea = new RECT();
	StartMenu(pArea);
	Rect area;
	area.x	= pArea->left*1.25;
	area.y = pArea->top;
	area.width =( pArea->right - pArea->left)*1.25;
	area.height = (pArea->bottom - pArea->top)*1.25;

	//Setup Screen Capture
	Veiwer vscreen(area);
	OCR ocr;
	Mat screen= vscreen.Capture();
	ocr.SetFont("C:\\Users\\Philip\\Documents\\Visual Studio 2015\\Projects\\Solution_finder\\fonts\\arial.PNG",16,16);
	ocr.textReconition("Result", screen);
	waitKey(1);

	waitKey();


	Screen ss;
	ss.height =vscreen.Capture().rows;
	ss.width = vscreen.Capture().cols;
	ss.image = Mat::zeros(ss.width, ss.height, CV_8UC3);
	ss.name ="Capture";
	ss.x=0;
	ss.y=0;
	SetWindow(ss.name, ss.image, ss.x, ss.y);

	//Setup Display
	Screen display;
	display.height =720;
	display.width = 1080;
	display.x = 40;
	display.y = 10;
	display.name = "Chart";
	display.image = Mat::zeros(display.width, display.height, CV_8UC3);

	//Setup Graph
	vector<fPoint> zero;
	zero.push_back({0,0});

	Graph  g("Chart",display.width, display.height);
	
	//Setup Problem
	int start = 32;
	int end=126;
	std::string answer = "helloworld";
	vector<float> input, output;
	for (size_t i = 0; i < answer.length(); i++){
		input.push_back(((float)answer[i]- start)/(end - start));
		std::cout << (char)((input[i]- start)/(end - start));
	}
	std::cout <<std::endl;
	
	//declare all EvoNet
	vector<EvoNet> fits;//EvoNet List
	double size,		rate,		hiddens,	hsize;
	double msize = 750, mrate= .001,mhiddens= 1,mhsize= 2;
	double xsize = 1000,xrate= .25,	xhiddens= 6,xhsize = 25;

	size = xsize;
	rate = .075;
	hiddens = 1;
	hsize = 1;
	fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	g.AddLine(zero);
	
	unsigned int count =0;
	vector<float> sbest;

	clock_t startt;
	double Passed;

	vector<string> outputs;
	int loadState = 0;

	//MAIN LOOP
	while (true)
	{
		//TIMER first
		startt = clock(); //Start timer

		//get window screen
		//char wnd_title[256];
		//std::cout<< GetWindowText(GetActiveWindow(), wnd_title, sizeof(wnd_title));
		//std::cout << wnd_title;
		//waitKey();
		ss.image =vscreen.Capture();

		/*for (size_t i = 0; i < answer.length(); i++) {
			std::cout << (char)((input[i]) * (end - start)+start);
		}*/

		//LOGIC
		for (size_t i = 0; i < fits.size(); i++)
		{
			fits[i].DoEpoch(input,false);
			fits[i].repopulate(.5);
			fits[i].updateStats(false);

			//Data

			fPoint p;
			p.x = count;
			p.y = fits[i].getBestScore();// /fits[i].getTime();
			g.AddData(p, i);

			//std::cout << "\tScore: " << p.y << " | ";

			cout << "Gen: " << count;

			string out;
			sbest =fits[i].getBestOut();
			for (size_t j = 0; j < answer.length(); j++)//get the output as text
				out +=(char)(sbest[j] * (end - start) + start);
			if(count <= 1)
				outputs.push_back(out);//get first output
			if (outputs.back()!= out)// if new output is diffrent from previous output
			{
				outputs.push_back(out);//add it
				cout << " | " << out << " "<< endl;
			}
			else
			{
				cout << "\b\b\b\b\b";
				for (size_t i = 0; i < std::to_string(count).length(); i++)
				{
					cout << "\b";
				}
			}
			
		}

		//PRINT
		//std::cout << std::endl;
		//std:: cout << "Gen: "<< count++ -1 << " | ";

		//DRAW
		g.DrawGraph();
		Mat scaled;
		//resize(ss.image,scaled, cvSize(0,0),0.75,1);
		imshow(ss.name, ss.image);
		waitKey(1);

		count++;

		Passed = (clock() - startt) / CLOCKS_PER_SEC;
		//std::cout << "\ttime:" << Passed;

		//_getch();
	}
	_getch();
	return 0;
}