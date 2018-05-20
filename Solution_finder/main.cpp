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
#include "Menu.h"
#include "OCR.h"
#include "Trainer.h"
	#include "Trainer.cpp"

using std::vector;
using namespace cv;

int main()
{
	cout << "Start"<< endl;

	//pre-initialization
	srand(static_cast <unsigned> (time(0)));

	clock_t startt=0;
	double frames=0;

	

	//bool done= false;
	bool exit = false;
	////TODO remove these varibles in place of dynamicly changeing ones
	std::string answer = "easy!";
	int sstart = 32;//min value
	int send = 126;//max value

	Nnet* pmainNet;//main Neural net for testing and loading
	//vector<EvoNet<lilNet>> lList;//lilNet List
	//lilNet* plmainNet;//main little Neural net for testing and loading


	unsigned int count =NULL;
	//int stale_max = 10000;
	//double stale_p = .2;//changed for testing , use .2
	//int staleCount = NULL;
	//double staleScore = NULL;

	//vector<double> sbest = {};

	Screen chartScr;
	Screen OCRScr;
	//OCR ocr;

	Rect area;
	RECT* pArea=nullptr;

	Menu mMenu;

	Graph g;

	//bool graphOn = true;

	pArea = new RECT();
	state option = NEW;
	option = mMenu.StartMenu(pArea, pmainNet);
	vector<double> input, output;
	//Rect area;
	area.x = pArea->left*1.25;
	area.y = pArea->top;
	area.width = (pArea->right - pArea->left)*1.25;
	area.height = (pArea->bottom - pArea->top)*1.25;

	cout << "Initialized" << endl;

	Veiwer vscreen(area);
	Trainer t(answer,input, option,area,chartScr);
	bool graphOn = true;

	cout << "Setup Up Done" << endl;

	if (option == NEW || option == CONTINUE)//if needs training
	{
		for (size_t i = 0; i < answer.length(); i++) {
			input.push_back(((double)answer[i] - sstart) / (send - sstart));
			std::cout << (char)((input[i] - sstart) / (send - sstart));
		}
		std::cout << std::endl;
	}
	//do what couldn't get started in setup
	g.Setup(chartScr.name, chartScr.width, chartScr.height);
	g.AddLine({ { 0,0 } });


	if (option == NEW || option == CONTINUE)//if needs training
	{
		while (!exit)
		{
			//train.train(OCRScr,vscreen,g,input, count,answer,graphOn);
			t.train((Screen)OCRScr,(Veiwer)vscreen, (Graph)g, (vector<double>)input,(unsigned int)count,(string)answer,(bool)graphOn);

			if (yesNoPromt("Do you want to prune network?"))
			{
				cout << "starting pruning" << endl;
			}
			cout << "Done Generating Network" << endl;
			mMenu.FinishTrainMenu(pArea, pmainNet);
		}
	}
	return 1;
}