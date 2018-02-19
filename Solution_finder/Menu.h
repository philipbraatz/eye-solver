#pragma once
#include <iostream>
#include <string>

#include <Windows.h>
#include <Winuser.h>

#include <vector>

#include "EvoNet.h"

#include "utility.h"

enum state
{
	NEW,
	CONTINUE,
	LOAD
};

class Menu
{
public:
	//Menu(Nnet* net)
	//{
	//	m_net = net;
	//}

	RECT* SizeWindow(RECT * area);

	state mainMenu(RECT * area, Nnet *& ref);

	state StartMenu(RECT * area, Nnet *&ref);

	state FinishTrainMenu(RECT* area, Nnet *&ref);



private:
	void MenuSizeWindow(RECT * area);
};

