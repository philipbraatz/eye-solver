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
	cout << "Start"<< endl;//debug

	//pre-initialization
	srand(static_cast <unsigned> (time(0)));

	clock_t startt=0;
	double frames=0;

	Mat trImage;
	string trText;

	bool exit = false;
	int sstart = 0;//min value
	int send = 1;//max value

	Nnet* pmainNet;//main Neural net for testing and loading


	unsigned int count =NULL;

	Screen chartScr;
	Screen OCRScr;
	Screen imgTrScr;
	//OCR ocr;

	Rect area;
	RECT* pArea=nullptr;

	Menu mMenu;

	Graph g;

	problem_type pt=IMAGE;
	//bool graphOn = true;

	pArea = new RECT();
	state option = NEW_DATA;
	option = mMenu.StartMenu(pArea, pmainNet,pt,trImage,trText);
	vector<double> input, output;
	//Rect area;
	area.x = pArea->left*1.25;
	area.y = pArea->top;
	area.width = (pArea->right - pArea->left)*1.25;
	area.height = (pArea->bottom - pArea->top)*1.25;

	cout << "Initialized" << endl;

	Veiwer vscreen(area);
	Trainer t(pt,trImage,trText,input, option,area,chartScr,imgTrScr);
	bool graphOn = true;

	cout << "Setup Up Done" << endl;

	if (option == NEW_TEXT || option == CONTINUE_TEXT)//text training
		for (size_t i = 0; i < trText.length(); i++) {
			input.push_back(((double)trText[i] - 32) / (126 - 32));
			std::cout << (char)((input[i] - 32) / (126 - 32));
		}
	else if (option == NEW_IMAGE || option == CONTINUE_IMAGE)//image training
		for (size_t i = 0; i < trImage.cols; i++)
			for (size_t j = 0; j < trImage.rows; j++)		
				for (size_t k = 0; k < trImage.channels(); k++)				
					input.push_back(((double)trImage.at<Vec3b>(i,j).val[k]) / 128);

	std::cout << std::endl;

	//do what couldn't get started in setup
	g.Setup(chartScr.name, chartScr.width, chartScr.height);
	g.AddLine({ { 0,0 } });

	while (!exit)//main loop
	{
		Mat blankM;
		string blankS;
		//train.train(OCRScr,vscreen,g,input, count,answer,graphOn);
		if(option == NEW_TEXT)
			t.train(
				(Screen)imgTrScr,
				(Veiwer)vscreen, 
				(Graph)g, 
				(vector<double>)input,
				(unsigned int)count,trText,blankM,
				(bool)graphOn);
		else if(option == NEW_IMAGE)
 			t.train(
				(Screen)imgTrScr, 
				(Veiwer)vscreen, 
				(Graph)g, 
				(vector<double>)input, 
				(unsigned int)count, blankS,
				(Mat)trImage, 
				(bool)graphOn);
		
		if (yesNoPromt("Do you want to prune network?"))
		{
			cout << "starting pruning" << endl;
			cout << "NOT IMPLIMENTED...skipping" << endl;
		}
		cout << "Done Generating Network" << endl;
		mMenu.FinishTrainMenu(pArea, pmainNet,pt,trImage,trText);
	}

	return 1;
}