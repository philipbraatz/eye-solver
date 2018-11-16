#pragma once
#include <iostream>
#include <string>

#include <Windows.h>
#include <Winuser.h>

#include <iostream>
#include <vector>

#include "../PEB.Display.Graph/Graph.h"
#include "EvoNet.h"
#include "../PEB.Utility/Utility.h"


enum state
{
	NEW_DATA,
	NEW_IMAGE,
	NEW_TEXT,
	CONTINUE_TEXT,
	CONTINUE_IMAGE,
	LOAD_NET,
	QUIT_TRAIN
};

class Menu
{
public:
	
	Menu();



	RECT* SizeWindow(RECT * area);

	state mainMenu(RECT * area, Nnet *&ref,NetFrame &netF, Mat &image, string &text);

	state StartMenu(RECT * area, Nnet *&ref, NetFrame &netF, Mat &image, string &text);

	state FinishTrainMenu(RECT* area, Nnet *&ref, NetFrame &netf,Mat &image,string &text);

	void _on_trackbar();//called by trackbar

private:
	void MenuSizeWindow(RECT * area);
	void LoadFile(Mat &image, string &text);
protected:
	string VERSION = "0.8.2";
	string NAME = "Screen Net";
	string AUTHOR = "Philip Braatz";

	//Sliders
	NetFrame SliderFrame;

	Slider hiddenSize;//# hidden layers
	Slider hiddenLayers;//size of a hidden layer
	Slider psize;//population size
	Slider rate;//mutation rate
};