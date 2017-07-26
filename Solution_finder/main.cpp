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
	state option;
	option = StartMenu(pArea);
	Rect area;
	area.x	= pArea->left*1.25;
	area.y = pArea->top;
	area.width =( pArea->right - pArea->left)*1.25;
	area.height = (pArea->bottom - pArea->top)*1.25;

	Veiwer vscreen(area);
	OCR ocr;
	Screen OCRScr;

	Screen chartScr;
	vector<fPoint> zero;

	if (option == NEW || option == CONTINUE)//if needs training
	{
		//ocr.SetFont("C:\\Users\\Philip\\Documents\\Visual Studio 2015\\Projects\\Solution_finder\\fonts\\arial.PNG",16,16);

		//setup OCR screen

		OCRScr.height =area.height;
		OCRScr.width = area.width;
		OCRScr.image = Mat::zeros(OCRScr.height, OCRScr.width, CV_8UC3);
		OCRScr.name ="Capture";
		OCRScr.x=0;
		OCRScr.y=0;
		//ocr.SetVeiwer(OCRScr.name, OCRScr.image);

		//Setup Graph screen

		chartScr.height =720;
		chartScr.width = 1080;
		chartScr.x = 40;
		chartScr.y = 10;
		chartScr.name = "Chart";
		chartScr.image = Mat::zeros(chartScr.width, chartScr.height, CV_8UC3);

		//Setup Graph

		zero.push_back({0,0});
	}

	Graph  g(chartScr.name, chartScr.width, chartScr.height);

	if (option == NEW || option == CONTINUE)//if needs training
	{
		//Setup Problem
		int start = 32;
		int end = 126;
		std::string answer = "abc123EFG";
		vector<float> input, output;
		for (size_t i = 0; i < answer.length(); i++) {
			input.push_back(((float)answer[i] - start) / (end - start));
			std::cout << (char)((input[i] - start) / (end - start));
		}
		std::cout << std::endl;

		//declare all EvoNet
		vector<EvoNet> fits;//EvoNet List
		double size, rate, hiddens, hsize;
		double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
		double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

		size = xsize;
		rate = .075;
		hiddens = 1;
		hsize = 1;
		fits.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
		g.AddLine(zero);

		vector<float> sbest;

		clock_t startt;
		int Passed;
		float frames;

		bool done = false;

		unsigned int count;
		///Learning loop
		
		//TIMER first
		startt = clock(); //Start timer

		while (!done)
		{

			OCRScr.image = vscreen.Capture();
			//ocr.textReconition(OCRScr.name, OCRScr.image);

			/*for (size_t i = 0; i < answer.length(); i++) {
				std::cout << (char)((input[i]) * (end - start)+start);
			}*/

			//LOGIC
			for (size_t i = 0; i < fits.size(); i++)
			{
				fits[i].DoEpoch(input, false);//set goal max =
				fits[i].repopulate(.5);
				fits[i].updateStats(false);

				count = fits[i].getGenCount();
				//Data

				fPoint p;
				p.x = count;
				p.y = fits[i].getBestScore();// /fits[i].getTime();
				g.AddData(p, i);

				//std::cout << "\tScore: " << p.y << " | ";

				if (count > 1)
				{
					count = fits[i].getGenCount();
					cout << "Gen: " << count;

					string out;
					sbest = fits[i].getCurrentBestOut();
					for (size_t j = 0; j < answer.length(); j++)//get the output as text
						out += (char)(sbest[j] * (end - start) + start);

					if (fits[i].getAllBestOut()[fits[i].getGenCount() - 2] != fits[i].getAllBestOut()[fits[i].getGenCount() - 1])// if new output is diffrent from previous output
					{
						cout << " | " << out << " " << endl;
					}
					else
					{
						cout << "\b\b\b\b\b\b\b\b\b\b\b\b";
						for (size_t i = 0; i < std::to_string(Passed).length(); i++)
						{
							cout << "\b";
						}

						cout << "\b\b\b\b\b";
						for (size_t i = 0; i < std::to_string(count).length(); i++)
							cout << "\b";
					}

					if (out == answer)
						done = true;
				}


			}

			//DRAW
			g.DrawGraph();
			Mat scaled;
			//resize(ss.image,scaled, cvSize(0,0),0.75,1);
			//imshow(OCRScr.name, OCRScr.image);
			waitKey(1);

			
			if ((clock() - startt) / CLOCKS_PER_SEC >= 1)
			{
				startt = clock();
				frames = Passed;
				Passed = 0;
			}
			else
			{
				Passed++;
			}
			std::cout << "Gen's/s:" << frames << " | ";

			//_getch();
		}
		cout << "Done Generating Network" << endl;
		if (yesNoPromt("would you like to save the Network?"))
		{
			fits.front().SaveBest("testBigSave");
		}
		cout << "Do you want to test the Network? (y/n)" << endl;

		_getch();
	}

	//Testing loop
	while (true)
	{

	}

	return 0;
}