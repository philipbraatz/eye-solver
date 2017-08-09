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

void Setup(vector<EvoNet> &List,Nnet *&mainNet, std::string &answer, vector<double> &input, int &sstart,int &send , Menu &mMenu, Rect &area,Graph g, RECT* &pArea, OCR &ocr, Screen &OCRScr, Screen &chartScr, state &option)
{
	//declare answer
	answer = "This is a lot harder!";
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
	//declare all EvoNet in List

	double size, rate, hiddens, hsize;
	double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
	double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

	//double size, rate, hiddens, hsize;
	//double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
	//double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

	size = xsize;
	rate = .075;
	hiddens = 1;
	hsize = 1;
	List.push_back(EvoNet(size, rate, answer.length(), hiddens, hsize, answer.length()));
	//g.AddLine(zero);

	pArea = new RECT();
	option = mMenu.StartMenu(pArea, mainNet);
	//Rect area;
	area.x = pArea->left*1.25;
	area.y = pArea->top;
	area.width = (pArea->right - pArea->left)*1.25;
	area.height = (pArea->bottom - pArea->top)*1.25;

	if (option == NEW || option == CONTINUE)//if needs training
	{
		for (size_t i = 0; i < answer.length(); i++) {
			input.push_back(((double)answer[i] - sstart) / (send - sstart));
			std::cout << (char)((input[i] - sstart) / (send - sstart));
		}
		std::cout << std::endl;
	}
}

int main()
{
	cout << "Start"<< endl;

	//pre-initialization
	srand(static_cast <unsigned> (time(0)));

	clock_t startt=0;
	int Passed=0;
	double frames=0;

	bool done= false;
	//TODO remove these varibles in place of dynamicly changeing ones
	std::string answer ="";
	int sstart = 0;
	int send= 0;

	vector<EvoNet> List;//EvoNet List
	Nnet* pmainNet;//main Neural net for testing and loading
	vector<double> input, output;


	unsigned int count =NULL;
	const int STALE_MAX = 10000;
	int staleCount = NULL;
	double staleScore = NULL;

	vector<double> sbest = {};

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
	//g.Setup(chartScr.name, chartScr.width, chartScr.height);
	//g.AddLine({ { 0,0 } });


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
				//g.AddData(p, i);

				//std::cout << "\tScore: " << p.y << " | ";

				count = List[i].getGenCount();
				cout << "Gen: " << count << " ";

				done = true;//true intel proven false
				string out;
				sbest = List[i].getCurrentBestOut();
				for (size_t j = 0; j < answer.length(); j++)//get the output as text
				{
					if (done && (int)answer[j] != sbest[j])
						done = false;
					out += (char)(sbest[j] * (send - sstart) + sstart);
				}
				if (count >= 2)
				{
					
					if (GetTotalDif(List[i].getCurrentBestOut(), List[i].getPreviousBestOut()) != 0)// if new output is diffrent from previous output
					{
						cout << " | " << out << " " << endl;
					}
					else	//remove previous character
					{
						cout << "\b\b\b\b\b\b\b\b\b\b\b";
						cout << "\b\b\b\b\b";
						for (size_t i = 0; i < std::to_string(Passed).length()+std::to_string(count).length(); i++)
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
			if (staleCount > STALE_MAX)
			{
				cout << endl << "Proggress has stopped, training is done" << endl;
				done = true;
			}


			//DRAW
			//g.DrawGraph();

			//INPUT
			if (GetKeyState(VK_NUMLOCK) > 0)//if key s is down
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

