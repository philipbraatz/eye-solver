#pragma once
#include <iostream>
#include <string>

#include <Windows.h>
#include <Winuser.h>

#include <iostream>
#include <vector>

#include "graphic.h"
#include "EvoNet.h"
#include "utility.h"



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
	//Menu(Nnet* net)
	//{
	//	m_net = net;
	//}

	RECT* SizeWindow(RECT * area);

	state mainMenu(RECT * area, Nnet *&ref, problem_type pt, Mat &image, string &text);

	state StartMenu(RECT * area, Nnet *&ref,problem_type pt, Mat &image, string &text);

	state FinishTrainMenu(RECT* area, Nnet *&ref, problem_type pt,Mat &image,string &text);

	

private:
	void MenuSizeWindow(RECT * area);
	void LoadFile(Mat &image, string &text);
};

