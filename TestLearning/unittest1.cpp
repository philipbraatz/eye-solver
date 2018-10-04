#include "stdafx.h"
#include "CppUnitTest.h"

using std::vector;
using namespace cv;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestLearning
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			std::cout << "Start" << std::endl;//debug

	//pre-initialization
			//srand(static_cast <unsigned> (time(0)));

			//clock_t startt = 0;
			//double frames = 0;

			//Mat trImage;
			//string trText;

			//bool exit = false;
			//int sstart = 0;//min value
			//int send = 1;//max value

			//Nnet* pmainNet;//main Neural net for testing and loading
			//NetFrame netF;//framework for neural network, used in loading

			//auto count = NULL;

			//Screen chartScr;
			//Screen OCRScr;
			//Screen imgTrScr;
			//OCR ocr;

			//Rect area;
			//RECT* pArea = nullptr;

			//TestMenu mMenu;
			// Menu mMenu;
			//Graph g;

			//netF.type = IMAGE;
			//bool graphOn = false;

			///pArea = new RECT();
			///state option = NEW_DATA;
			///option = mMenu.StartMenu(pArea, pmainNet, netF, trImage, trText);//start menu
			vector<double> input, output;
			///area.x = pArea->left*1.25;
			///area.y = pArea->top;
			///area.width = (pArea->right - pArea->left)*1.25;
			///area.height = (pArea->bottom - pArea->top)*1.25;

			cout << "Initialized" << endl;

			//Veiwer vscreen(area);
			///Trainer<Nnet> t(netF, trImage, trText, input, option, area, chartScr, imgTrScr);//create trainer

			cout << "Setup Up Done" << endl;

//			if (option == NEW_TEXT || option == CONTINUE_TEXT)//text training
//				for (size_t i = 0; i < trText.length(); i++) {
//					input.push_back(((double)trText[i] - 32) / (126 - 32));
//					std::cout << (char)((input[i] - 32) / (126 - 32));
//				}
//			else if (option == NEW_IMAGE || option == CONTINUE_IMAGE)//image training
//				for (size_t i = 0; i < trImage.cols; i++)
//					for (size_t j = 0; j < trImage.rows; j++)
//						for (size_t k = 0; k < trImage.channels(); k++)
//							input.push_back(((double)trImage.at<Vec3b>(i, j).val[k]) / 128);
//
			std::cout << std::endl;

			//do what couldn't get started in setup
			///g.Setup(chartScr.name, chartScr.width, chartScr.height);
			///g.AddLine({ { 0,0 } });
//
//			while (!exit)//main loop
//			{
//				Mat blankM;
//				string blankS;
//				//train.train(OCRScr,vscreen,g,input, count,answer,graphOn);
//				if (option == NEW_TEXT)
//					t.train(
//					(Screen)imgTrScr,
//						(Veiwer)vscreen,
//						(Graph)g,
//						(vector<double>)input,
//						(int)count, trText, blankM,
//						(bool)graphOn);
//				else if (option == NEW_IMAGE)
//					t.train(
//					(Screen)imgTrScr,
//						(Veiwer)vscreen,
//						(Graph)g,
//						(vector<double>)input,
//						(int)count, blankS,
//						(Mat)trImage,
//						(bool)graphOn);
//
//				if (true)
//				{
//					cout << "starting pruning" << endl;
//					cout << "NOT IMPLIMENTED...skipping" << endl;
//				}
//				cout << "Done Generating Network" << endl;
//				mMenu.FinishTrainMenu(pArea, pmainNet, netF, trImage, trText);
//			}
		}

	};
}