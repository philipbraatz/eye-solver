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

#include "../PEB.Display.Graph/Graph.h"

#include "EvoNet.h"
#include "EvoNet.cpp"
#include <Nnet.h>
#include "../PEB.Utility/Utility.h"
#include "capture.h"
//#include "Menu.h"
#include "Tracker.h"
#include "OCR.h"

template<class tNet>
class Trainer
{
protected:

	bool done = false;
	bool exit = false;
	int sstart = 0;
	clock_t startt = 0;
	int send = 0;
	int frames = 0;
	int passed = 0;

	//vector<EvoNet<Nnet>> List;//EvoNet List
	//Nnet* pmainNet;//main Neural net for testing and loading
	vector<EvoNet<tNet>> List;//lilNet List
	lilNet* pmainNet;//main little Neural net for testing and loading
	vector<double> output;


	int counter = NULL;
	int stale_Max = 500;
	double STALE_P = .2;//changed for testing , use .2
	int staleCount = NULL;
	double staleScore = NULL;

	vector<double> sbest = {};

	Screen chartScr;
	Screen OCRScr;
	Tracker track1;
	OCR ocr;

	Rect area;
	RECT* pArea = nullptr;

	//Menu mMenu;

	//state option = NEW;

public:
	Trainer( 
		NetFrame netf,
		Mat &imgAnswer,
		std::string &txtAnswer,
		vector<double> &input,
		state &option,
		Rect area,
		Screen &chartScr,
		Screen &outputScr
	)
	{
		if (NEW_TEXT)
		{
			sstart = 32;//ascii start
			send = 126;//ascii end
		}
		else if (NEW_IMAGE)
		{
			sstart = 0;//color white
			send = 128;//color black
		}
			

				   //declare answer
		//answer = i_answer;
		//input = i_input;
		//option = i_option;

		vector<fPoint> zero;

		if (option == NEW_TEXT || option == CONTINUE_TEXT ||
			option == NEW_IMAGE || option == CONTINUE_IMAGE
			)//if needs training
		{
			///ocr.SetFont("C:\\Users\\Philip\\Documents\\Visual Studio 2015\\Projects\\Solution_finder\\fonts\\arial.PNG", 16, 16);

			
			
			if (area.height == 0)//error handle
				cout << "ERROR area not set";

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
			chartScr.x = 70;
			chartScr.y = 10;
			chartScr.name = "Chart";
			chartScr.image = Mat::zeros(chartScr.width, chartScr.height, CV_8UC3);

			outputScr.height = 200;
			outputScr.width = 350;
			outputScr.x = 40;
			outputScr.y = 200;
			outputScr.name = "Network Output";
			outputScr.image = Mat::zeros(outputScr.width, outputScr.height, CV_8UC3);

			//Setup Graph

			zero.push_back({ 0,0 });
		}
		//declare all EvoNet in List

		double size, rate, hiddens, hsize;
		double msize = 350, mrate = .001, mhiddens = 1, mhsize = 2;
		double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;

		//double size, rate, hiddens, hsize;
		//double msize = 750, mrate = .001, mhiddens = 1, mhsize = 2;
		//double xsize = 1000, xrate = .25, xhiddens = 6, xhsize = 25;
		
		size = msize;
		rate = .1;
		hiddens = 2;
		hsize = 4;
		if(option == NEW_TEXT)
			List.push_back(EvoNet<tNet>(size, rate, txtAnswer.length(),
				hiddens, hsize, txtAnswer.length(),netf.type));
		else if (option == NEW_IMAGE)
		{
			List.push_back(EvoNet<tNet>(size, rate, imgAnswer.cols*imgAnswer.rows*imgAnswer.channels(),
				hiddens, hsize, imgAnswer.cols*imgAnswer.rows*imgAnswer.channels(), netf.type));
		}
		//g.AddLine(zero);
	}

	void train(Screen &OCRScr, Veiwer &vscreen, Graph &g,
		vector<double> &input, int &i_count,
		std::string &txtAnswer, Mat &imgAnswer, bool &graphOn)
	{
		bool prune = false;

		//TIMER first
		this->startt = clock();//Start timer
		bool done = false;
		while (!done)
		{
			OCRScr.image = vscreen.Capture();
			//ocr.textReconition(OCRScr.name, OCRScr.image);

			//LOGIC

			for (int i = 0; i < List.size(); i++)
			{
				List[i].DoEpoch(input, false, true,prune);//set goal max =
				List[i].repopulate(0.5);
				List[i].updateStats(true);

				counter = List[i].getGenCount();
				//Data

				fPoint p;
				p.x = counter;
				p.y = List[i].getBestScore();// /List[i].getTime();
				g.AddData(p, i);

				double successs_Rate = List[i].

					//std::cout << "\tScore: " << p.y << " | ";// old

				counter = 0;//List[i].getGenCount();
				//cout << "Gen: " << count << " ";//old
				track1.display();

				done = true;
				string out;
				sbest = List[i].getCurrentBestOut();
				if(txtAnswer !="")
					for (auto j = 0; j < txtAnswer.length(); j++)//get the output as text
					{
						if (done && (int)txtAnswer[j] != sbest[j])
							done = false;
						int numDebug = (int)abs(sbest[j] * (send - sstart) + sstart);
						out += (char)(abs(sbest[j] * (send - sstart) + sstart));
					}
				if (imgAnswer.data)
				{
					Mat outputImg(imgAnswer.cols, imgAnswer.rows,CV_8UC3,Scalar(0,0,0));
					for (size_t i = 0; i < imgAnswer.cols; i++)
					{
						for (size_t j = 0; j < imgAnswer.rows; j++)
						{
							Vec3b color;
							for (size_t k = 0; k < imgAnswer.channels(); k++)
							{
								done = false;
								//({0,0}[0,0][0,1][0,2]),({1,0}[0,4][0,5][0,6]),({2,0}[0,7][0,8][0,9])			=C(R*cl+r)+c
								//({0,1}[0,10][0,11][0,12]),({1,1}[0,13][0,14][0,15]),({2,1}[0,16][0,17][0,18]) [rows]*[channels]*col+row*[channels]+channel
								//({0,2}[0,19][0,20][0,21]),({1,2}[0,22][0,23][0,24]),({2,2}[0,25][0,26][0,27]) = R*C*cl+r*C+c
								outputImg.at<Vec3b>(Point(i, j))[k] = (int)sbest[imgAnswer.channels()*(imgAnswer.rows*i + j) + k];
							}
						}
					}
					out +=  to_string((int)(List[i].getAveScore()));

					imshow("Network Output", outputImg);
					waitKey(1);
				}
				//if (count >= 2)
				//{
				//	if (count == 2)
				//		cout << " | " << out << " " << endl;
				//	if (GetTotalDif(List[i].getCurrentBestOut(), List[i].getPreviousBestOut()) != 0)// if new output is diffrent from previous output
				//	{
				//		cout << " | " << out << " " << endl;
				//	}
				//	else	//remove previous character
				//	{
				//		cout << "\b\b\b\b\b\b\b\b\b\b\b";
				//		cout << "\b\b\b\b\b";
				//		for (auto i = 0; i < std::to_string(passed).length() + std::to_string(count).length() + out.length(); i++)
				//			cout << "\b";
				//	}
				//}

				//if (answer.compare(out))//if anwer and out are the same string
				//	cout << "perfected Network! training done.";
				//	done = true;
				track1.update(frames,counter,p.y,1,out);

				if (List[i].getBestScore() == staleScore)
					staleCount++;
				else
				{
					staleCount = 0;
					staleScore = List[i].getBestScore();
				}
				
			}
			if (staleCount > stale_Max)
			{
				if (staleCount > List.front().getGenCount() * STALE_P)
				{
					if (prune)
					{
						cout << "pruning is done" << endl;
						List.front().SaveBest("complete");
						cout << endl << "Network has been saved!" << endl;
						done = true;
					}
					else
					{
						cout << endl << "Proggress has stopped";
						cout << ", Pruning Started" << endl;
						//TODO add pruning
						prune = true;
						for (size_t i = 0; i < List.size(); i++)
						{
							List[i].PruneAll();
						}
					}



					
				}
				else
					stale_Max = (int)List.front().getGenCount() * STALE_P;
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
			};
		}
	}
};

