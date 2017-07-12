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

using std::vector;

int main()
{
	//pre-initialization

	///hide console
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	srand(static_cast <unsigned> (time(0)));

	//Setup Display
	Screen display;
	display.height = 720;
	display.width = 1080;
	display.x = 100;
	display.y = 50;
	display.name = "chart";
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
	double size,rate,hiddens,hsize;
	double msize =750, mrate=.001, mhiddens=1, mhsize=2;
	double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

	size = msize;
	rate = .075;
	hiddens = 1;
	hsize = 1;
	fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	g.AddLine(zero);
	
	unsigned int count =0;
	vector<float> sbest;

	clock_t startt;
	double Passed;

	//MAIN LOOP
	while (true)
	{
		//TIMER first
		startt = clock(); //Start timer

		for (size_t i = 0; i < answer.length(); i++) {
			std::cout << (char)((input[i]) * (end - start)+start);
		}

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

			std::cout << "\tScore: " << p.y << " | ";

			sbest =fits[i].getBestOut();
			for (size_t j = 0; j < answer.length(); j++)
				std::cout << (char)(sbest[j] * (end - start) + start);
		}

		//PRINT
		std::cout << std::endl;
		std:: cout << "Gen: "<< count++ -1 << " | ";

		//DRAW
		g.DrawGraph();

		Passed = (clock() - startt) / CLOCKS_PER_SEC;
		//std::cout << "\ttime:" << Passed;

		//_getch();
	}
	_getch();
	return 0;
}