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
	g.AddLine(zero);		//Best line 0
	g.AddLine(zero);		//Average line 1
	
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
	

	EvoNet group(250, .05, answer.length(), 3, answer.length(), answer.length());

	unsigned int count =0;
	vector<float> sbest;

	//MAIN LOOP
	while (true)
	{
		
		//LOGIC
		group.DoEpoch(input);
		group.repopulate(.1);
		group.updateStats();

		//PRINT
		std:: cout << count++ -1 << " | ";

		sbest =group.getBestOut();
		for (size_t i = 0; i < answer.length(); i++)
		{
			std::cout << (char)(sbest[i] * (end - start) + start);
		}


		//Data
		std::cout << std::endl;
		fPoint p;
		p.x = count; 

		p.y = group.getBestScore();
		g.AddData(p,0);

		p.y = group.getAveScore();
		g.AddData(p, 1);

		//DRAW
		g.DrawGraph();


		//_getch();
	}
	_getch();
	return 0;
}