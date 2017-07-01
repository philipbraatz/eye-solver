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
		input.push_back(((int)answer[i]- start)/(end - start));
		std::cout << (char)((input[i]- start)/(end - start));
	}
	std::cout <<std::endl;
	
	vector<EvoNet> fits;
	double size,rate,hiddens,hsize;
	double msize =10, mrate=.001, mhiddens=1, mhsize=2;
	double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

	size = msize;
	rate = .05;
	hiddens = 2;
	hsize = 11;
	fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	g.AddLine(zero);

	size = msize*10;
	rate = .05;
	hiddens = 2;
	hsize = 15;
	fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	g.AddLine(zero);

	size = msize*25;
	rate = .05;
	hiddens = 2;
	fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	g.AddLine(zero);

	size = msize * 50;
	rate = .05;
	hiddens = 2;
	hsize = 15;
	fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	g.AddLine(zero);

	size = msize * 100;
	rate = .05;
	hiddens = 2;
	hsize = 15;
	fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	g.AddLine(zero);

	
	unsigned int count =0;
	vector<float> sbest;

	//MAIN LOOP
	while (true)
	{
		
		//LOGIC
		for (size_t i = 0; i < fits.size(); i++)
		{
			fits[i].DoEpoch(input);
			fits[i].repopulate(.5);
			fits[i].updateStats();

			sbest =fits[i].getBestOut();
			
			for (size_t j = 0; j < answer.length(); j++)
			std::cout << (char)(sbest[j] * (end - start) + start);


		}

		//PRINT
		std::cout << std::endl;
		std:: cout << count++ -1 << " | ";



		//Data

		fPoint p;
		p.x = count;

		for (size_t i = 0; i < fits.size(); i++)
		{
			p.y = fits[i].getBestScore();
			g.AddData(p, i);
		}

		//DRAW
		g.DrawGraph();

		//_getch();
	}
	_getch();
	return 0;
}