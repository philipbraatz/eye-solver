//#include "stdafx.h"
#include "TestMenu.h"
//
//
//state TestMenu::StartMenu(RECT * area, Nnet *&ref, NetFrame &netf, Mat &image, string &text)
//{
//	cout << "--------------------Start----------------------------" << endl;//DEBUG
//	cout << "TEST Solution Finder " << VERSION << "t:" << std::endl;
//
//	return mainMenu(area, ref, netf, image, text);
//}
//
//state TestMenu::mainMenu(RECT * area, Nnet *&ref, NetFrame &netF_, Mat &image, string &text)
//{
//	cout << "--------------------MAIN----------------------------" << endl;
//
//	//int chose = 0;//SAFTY not working
//	while (true)//LOAD / NEW
//	{
//		if (false)
//		{
//			
//			if (false)///Load Test
//			{
//				//Nnet n("complete.bin");
//				Nnet n("manual.bin");
//				ref = &n;
//
//				//train
//				SizeWindow(area);
//				netF_ = SliderFrame;
//				return CONTINUE_TEXT;
//			}		
//		}
//		else//New Network
//		{
//			LoadFile(image, text);
//
//			SizeWindow(area);
//			if (text != "")
//				return NEW_TEXT;
//			else if (image.data)
//				return NEW_IMAGE;
//			else
//				return NEW_DATA;
//		}
//	}
//}
//
//void TestMenu::LoadFile(Mat &image, string &text)
//{
//	string file ="test.txt";//test file
//	//string file = "test.png";
//	string type;
//
//	//read file
//	if (file.length() > 3)
//		type = file.substr(file.length() - 3, 3);
//	else
//	{
//		cout << "file type not supported";
//		type = "txt";
//		text = file.append("0");//signed with 0 for short error
//	}
//	if (type == "txt")
//	{
//		image = NULL;
//		text = file.substr(0, file.length() - 4);//TODO: make it read the file
//	}
//	else if (
//		type == "png" ||
//		type == "peg" ||
//		type == "jpg" ||
//		type == "jpe" ||
//		type == "bmp"
//		)
//	{
//		text = "";
//		image = imread("train_data//" + file, CV_LOAD_IMAGE_COLOR);
//		if (!image.data)
//		{
//			cout << "ERROR: Could not open image from " + ("train_data/" + file);
//			waitKey();
//		}
//		SetWindow("load image test", image, 50, 50);
//		waitKey();
//
//	}
//	else
//	{
//		cout << "File type " << type << " not supported, will be treated as text\n";
//		type = "txt";
//	}
//}
//
//state TestMenu::FinishTrainMenu(RECT* area, Nnet *&ref, NetFrame &netf, Mat &image, string &text)
//{
//	cout << "--------------------END----------------------------" << endl;
//	cout << "Done Generating Network" << std::endl;
//	cout << "results:\t" << true;
//	return state::QUIT_TRAIN;
//}
