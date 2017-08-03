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

void Setup(vector<EvoNet> &List,Nnet *&mainNet, std::string &answer, vector<float> &input, int &sstart,int &send , Menu &mMenu, Rect &area,Graph g, RECT* &pArea, OCR &ocr, Screen &OCRScr, Screen &chartScr, state &option)
{
	//declare answer
	answer = "abc123EFG";
	sstart = 32;
	send = 126;

	vector<fPoint> zero;

	if (option == NEW || option == CONTINUE)//if needs training
	{
		///ocr.SetFont("C:\\Users\\Philip\\Documents\\Visual Studio 2015\\Projects\\Solution_finder\\fonts\\arial.PNG", 16, 16);

		//setup OCR screen

		OCRScr.height = area.height;
		OCRScr.width = area.width;
		OCRScr.image = Mat::zeros(OCRScr.height, OCRScr.width, CV_8UC3);
		OCRScr.name = "Capture";
		OCRScr.x = 0;
		OCRScr.y = 0;
		///ocr.SetVeiwer(OCRScr.name, OCRScr.image);

		//Setup Graph screen

		chartScr.height = 720;
		chartScr.width = 1080;
		chartScr.x = 40;
		chartScr.y = 10;
		chartScr.name = "Chart";
		chartScr.image = Mat::zeros(chartScr.width, chartScr.height, CV_8UC3);

		//Setup Graph

		zero.push_back({ 0,0 });
	}
	///pg = &g;
	//declare all EvoNet in List
	//vector<EvoNet> List;//EvoNet List



	double size, rate, hiddens, hsize;
	double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
	double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

	size = xsize;
	rate = .075;
	hiddens = 1;
	hsize = 1;
	List.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	//g.AddLine(zero);

	//start menu

	//Menu mMenu;

	pArea = new RECT();
	option = mMenu.StartMenu(pArea, mainNet);
	//Rect area;
	area.x = pArea->left*1.25;
	area.y = pArea->top;
	area.width = (pArea->right - pArea->left)*1.25;
	area.height = (pArea->bottom - pArea->top)*1.25;

	//Veiwer vscreen(area);
	//OCR ocr;
	//Screen OCRScr;

	//Screen chartScr;





	if (option == NEW || option == CONTINUE)//if needs training
	{
		//Setup Problem
		//int start = 32;
		//int end = 126;
		//std::string answer = "abc123EFG";
		//vector<float> input, output;
		for (size_t i = 0; i < answer.length(); i++) {
			input.push_back(((float)answer[i] - sstart) / (send - sstart));
			std::cout << (char)((input[i] - sstart) / (send - sstart));
		}
		std::cout << std::endl;


		//vector<float> sbest;

		//clock_t startt;
		//int Passed;
		//float frames;

		//bool done = false;

		//unsigned int count;
		//const int STALE_MAX = 10000;
		//int staleCount = 0;
		//float staleScore = 0;
	}
}

int main()
{
	cout << "Start"<< endl;

	//pre-initialization
	srand(static_cast <unsigned> (time(0)));

	/////hide console
	////ShowWindow(GetConsoleWindow(), SW_HIDE);



	////declare all EvoNet in List
	//vector<EvoNet> List;//EvoNet List

	//double size, rate, hiddens, hsize;
	//double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
	//double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

	//size = xsize;
	//rate = .075;
	//hiddens = 1;
	//hsize = 1;
	//List.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	//g.AddLine(zero);

	////start menu

	//Menu mMenu(List);

	//RECT* pArea = new RECT();
	//state option;
	//option = mMenu.StartMenu(pArea,loader);
	//Rect area;
	//area.x	= pArea->left*1.25;
	//area.y = pArea->top;
	//area.width =( pArea->right - pArea->left)*1.25;
	//area.height = (pArea->bottom - pArea->top)*1.25;

	//Veiwer vscreen(area);
	//OCR ocr;
	//Screen OCRScr;

	//Screen chartScr;
	//vector<fPoint> zero;

	//if (option == NEW || option == CONTINUE)//if needs training
	//{
	//	ocr.SetFont("C:\\Users\\Philip\\Documents\\Visual Studio 2015\\Projects\\Solution_finder\\fonts\\arial.PNG",16,16);

	//	//setup OCR screen

	//	OCRScr.height =area.height;
	//	OCRScr.width = area.width;
	//	OCRScr.image = Mat::zeros(OCRScr.height, OCRScr.width, CV_8UC3);
	//	OCRScr.name ="Capture";
	//	OCRScr.x=0;
	//	OCRScr.y=0;
	//	ocr.SetVeiwer(OCRScr.name, OCRScr.image);

	//	//Setup Graph screen

	//	chartScr.height =720;
	//	chartScr.width = 1080;
	//	chartScr.x = 40;
	//	chartScr.y = 10;
	//	chartScr.name = "Chart";
	//	chartScr.image = Mat::zeros(chartScr.width, chartScr.height, CV_8UC3);

	//	//Setup Graph

	//	zero.push_back({0,0});
	//}

	//Graph  g(chartScr.name, chartScr.width, chartScr.height);

	//if (option == NEW || option == CONTINUE)//if needs training
	//{
	//	//Setup Problem
	//	int start = 32;
	//	int end = 126;
	//	std::string answer = "abc123EFG";
	//	vector<float> input, output;
	//	for (size_t i = 0; i < answer.length(); i++) {
	//		input.push_back(((float)answer[i] - start) / (end - start));
	//		std::cout << (char)((input[i] - start) / (end - start));
	//	}
	//	std::cout << std::endl;


	//	vector<float> sbest;

	//	clock_t startt;
	//	int Passed;
	//	float frames;

	//	bool done = false;

	//	unsigned int count;
	//	const int STALE_MAX = 10000;
	//	int staleCount =0;
	//	float staleScore=0;
	//	///Learning loop
	//	
	//	cout << endl << endl;

	clock_t startt=0;
	int Passed=0;
	float frames=0;

	bool done= false;
	//TODO remove these varibles in place of dynamicly changeing ones
	std::string answer= "abc123EFG";
	int sstart = 32;
	int send= 126;

	vector<EvoNet> List;//EvoNet List
	Nnet* pmainNet;//main Neural net for testing and loading
	vector<float> input, output;


	unsigned int count =NULL;
	const int STALE_MAX = 10000;
	int staleCount = NULL;
	float staleScore = NULL;

	vector<float> sbest = {};

	Screen chartScr;
	Screen OCRScr;
	OCR ocr;

	Rect area;
	RECT* pArea=nullptr;

	Menu mMenu;

	Graph g;

	state option = NEW;

	cout << "Initialized" << endl;

	Setup(List, pmainNet,  answer,input, sstart, send, mMenu, area,g, pArea, ocr, OCRScr,chartScr,option);

	cout << "Setup Up Done" << endl;
	//After setup
	//do what couldn't get started in setup
	Veiwer vscreen(area);
	g.Setup(chartScr.name, chartScr.width, chartScr.height);
	g.AddLine({ { 0,0 } });


	if (option == NEW || option == CONTINUE)//if needs training
	{
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

			for (int i = 0; i < List.size(); i++)
			{
				List[i].DoEpoch(input, false, true);//set goal max =
				List[i].repopulate(0.5);
				List[i].updateStats(false);

				count = List[i].getGenCount();
				//Data

				fPoint p;
				p.x = count;
				p.y = List[i].getBestScore();// /List[i].getTime();
				g.AddData(p, i);

				//std::cout << "\tScore: " << p.y << " | ";

				if (count > 1)
				{
					count = List[i].getGenCount();
					cout << "Gen: " << count;

					string out;
					sbest = List[i].getCurrentBestOut();
					for (size_t j = 0; j < answer.length(); j++)//get the output as text
						out += (char)(sbest[j] * (send - sstart) + sstart);

					if (List[i].getAllBestOut()[List[i].getGenCount() - 2] != List[i].getAllBestOut()[List[i].getGenCount() - 1])// if new output is diffrent from previous output
					{
						cout << " | " << out << " " << endl;
					}
					else
					{
						cout << "\b\b\b\b\b\b\b\b\b\b";
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

				if (List[i].getBestScore() == staleScore)
					staleCount++;
				else
				{
					staleCount = 0;
					staleScore = List[i].getBestScore();
				}
			}
			if (staleCount > STALE_MAX)
			{
				cout << endl << "Proggress has stopped, training is done" << endl;
				done = true;
			}


			//DRAW
			g.DrawGraph();
			Mat scaled;
			//resize(ss.image,scaled, cvSize(0,0),0.75,1);
			//imshow(OCRScr.name, OCRScr.image);
			waitKey(1);

			//INPUT
			if (GetKeyState('s') > 0)//if key s is down
			{
				if (yesNoPromt("Do you want to save?"))
				{
					List.front().SaveBest("manual save");

					if (yesNoPromt("Do you want to test?"))
					{
						done = true;
					}
				}
			}


			//CLOCK
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
			std::cout << "Gen/s:" << frames << " | ";

		}

	cout << "Done Generating Network" << endl;
	mMenu.FinishTrainMenu(pArea, pmainNet);
	}

}

