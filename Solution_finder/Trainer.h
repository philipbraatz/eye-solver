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
#include "EvoNet.cpp"
#include "Nnet.h"
#include "utility.h"
#include "capture.h"
//#include "Menu.h"
#include "OCR.h"

class Trainer
{
//protected:

	bool done = false;
	bool exit = false;
	//TODO remove these varibles in place of dynamicly changeing ones
	//std::string answer = "";
	int sstart = 0;
	clock_t startt = 0;
	int send = 0;
	int frames = 0;
	int passed = 0;

	vector<EvoNet<Nnet>> List;//EvoNet List
	Nnet* pmainNet;//main Neural net for testing and loading
	vector<EvoNet<lilNet>> lList;//lilNet List
	lilNet* plmainNet;//main little Neural net for testing and loading
	vector<double> output;


	unsigned int count = NULL;
	int stale_max = 10000;
	double stale_p = .2;//changed for testing , use .2
	int staleCount = NULL;
	double staleScore = NULL;

	vector<double> sbest = {};

	Screen chartScr;
	Screen OCRScr;
	OCR ocr;

	Rect area;
	RECT* pArea = nullptr;

	//Menu mMenu;

	//state option = NEW;

public:
	Trainer(std::string &answer, vector<double> &input, state &option, Rect area, Screen &chartScr)//(vector<EvoNet<Nnet>> &List, Nnet *&mainNet, std::string &answer,
																		 //vector<double> &input, int &sstart, int &send, Menu &mMenu,
																		 //Rect &area, Graph g, RECT* &pArea, OCR &ocr, Screen &OCRScr, Screen &chartScr, state &option)
	{

		sstart = 32;//answer min value
		send = 126;//answer max value

				   //declare answer
		//answer = i_answer;
		//input = i_input;
		//option = i_option;

		vector<fPoint> zero;

		if (option == NEW || option == CONTINUE)//if needs training
		{
			///ocr.SetFont("C:\\Users\\Philip\\Documents\\Visual Studio 2015\\Projects\\Solution_finder\\fonts\\arial.PNG", 16, 16);

			
			
			if (area.height == 0)//error handle
				cout << "ERROR area too small";

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
		//declare all EvoNet in List

		double size, rate, hiddens, hsize;
		double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
		double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

		//double size, rate, hiddens, hsize;
		//double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
		//double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

		size = msize;
		rate = .075;
		hiddens = 2;
		hsize = 6;
		List.push_back(EvoNet<Nnet>(size, rate, answer.length(), hiddens, hsize, answer.length()));
		//g.AddLine(zero);
	}

	void train(Screen &OCRScr, Veiwer &vscreen, Graph &g,
		vector<double> &input, unsigned int &i_count,
		std::string &answer, bool &graphOn)
	{
		//OCRScr = i_OCRScr;
		//input = i_input;
		//answer = i_answer;
		//graphOn = i_graphOn;

		//TIMER first
		this->sstart = clock();//Start timer
		bool done = false;
		while (!done)
		{
			OCRScr.image = vscreen.Capture();
			//ocr.textReconition(OCRScr.name, OCRScr.image);

			/*for (unsigned int i = 0; i < answer.length(); i++) {
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

				count = List[i].getGenCount();
				cout << "Gen: " << count << " ";

				done = true;//true intel proven false
				string out;
				sbest = List[i].getCurrentBestOut();
				for (unsigned int j = 0; j < answer.length(); j++)//get the output as text
				{
					if (done && (int)answer[j] != sbest[j])
						done = false;
					out += (char)(sbest[j] * (send - sstart) + sstart);
				}
				if (count >= 2)
				{
					if (count == 2)
						cout << " | " << out << " " << endl;
					if (GetTotalDif(List[i].getCurrentBestOut(), List[i].getPreviousBestOut()) != 0)// if new output is diffrent from previous output
					{
						cout << " | " << out << " " << endl;
					}
					else	//remove previous character
					{
						cout << "\b\b\b\b\b\b\b\b\b\b\b";
						cout << "\b\b\b\b\b";
						for (unsigned int i = 0; i < std::to_string(passed).length() + std::to_string(count).length(); i++)
							cout << "\b";
					}
				}


				//if (answer.compare(out))//if anwer and out are the same string
				//	cout << "perfected Network! training done.";
				//	done = true;

				if (List[i].getBestScore() == staleScore)
					staleCount++;
				else
				{
					staleCount = 0;
					staleScore = List[i].getBestScore();
				}
			}
			if (staleCount > stale_max)
			{
				if (stale_max > List.front().getGenCount() * stale_p)
				{
					cout << endl << "Proggress has stopped";
					cout << ", Pruning Started";
					//TODO add pruning
					cout << "training is done" << endl;
					List.front().SaveBest("complete");
					cout << endl << "Network has been saved!" << endl;
					done = true;
				}
				else
					stale_max = List.front().getGenCount() * stale_p;
			}


			//DRAW
			if (graphOn)
				g.DrawGraph();//slows down a bit

							  //INPUT
			if (GetKeyState(VK_RIGHT) > 0)//if key right is down
			{
				if (yesNoPromt("Do you want to save?"))
				{
					List.front().SaveBest("manual");

					if (yesNoPromt("Exit to main menu?"))
					{
						done = true;
					}
				}
			}
			else if (GetKeyState(VK_LEFT) > 0)//if key left is down
			{
				if (graphOn)
					graphOn = false;
				else
					graphOn = true;
			}


			//CLOCK
			if ((clock() - startt) / CLOCKS_PER_SEC >= 1)
			{
				startt = clock();
				frames = passed;
				passed = 0;
			}
			else
			{
				passed++;
			}
			;
			std::cout << "Gen/s:" << std::setprecision(0) << frames << " | ";

		}
	}


};

