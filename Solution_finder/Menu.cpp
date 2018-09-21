#pragma once
#include "Menu.h"



using std::cout;
using std::cin;
using namespace cv;

RECT* Menu::SizeWindow(RECT * area)
{
	HWND handle;
	Mat blank = Mat::zeros({100,200},1 );
	SetWindow("Size Me", blank, 100, 100);
	cout << "Set the size of the \"Size Me\" window and press enter";
	imshow("Size Me", blank);
	waitKey();

	if (handle = FindWindow(NULL, TEXT("Size Me"))) {
		//std::cout << "Found!"; 
		GetWindowRect(handle, area);
		DestroyWindow(handle);
	}
	else {cout << "not found";}
	return area;
}

void Menu::LoadFile(Mat &image, string &text)
{
	string file;
	string type;
	cout << "Enter a file name you want to train on within the train_data folder\n";
	cin >> file;
	if (file.length() > 3)
		type = file.substr(file.length() - 3, 3);
	else
	{
		cout << "file type not supported";
		type = "txt";
		text = file.append("0");//signed with 0 for short error
	}
	if (type == "txt")
	{
		image = NULL;
		text = file.substr(0,file.length()-4);
	}
	else if (
		type == "png"||
		type == "peg"||
		type == "jpg"||
		type == "jpe"||
		type == "bmp"
		)
	{
		text = "";
		image = imread("train_data//"+file, CV_LOAD_IMAGE_COLOR);
		if (!image.data)
		{
			cout << "ERROR: Could not open image from "+("train_data/"+file);
			waitKey();
		}
		SetWindow("load image test",image,50,50);
		waitKey();

	}
	else
	{
		cout << "File type " << type << " not supported, will be treated as text\n";
		type = "txt";
		//text = file.append("");
	}
}

state Menu::mainMenu(RECT * area, Nnet *&ref, problem_type pt,Mat &image,string &text)
{
	cout << "--------------------MAIN----------------------------" << endl;

	int chose = 0;//SAFTY not working
	while (true)//LOAD / NEW
	{
		if (yesNoPromt("Would you like to load a Network?"))
		{//Load Network
			if (yesNoPromt("Load Complete(y) or Manual(n) save?")) {
				Nnet n("complete.bin");
				ref = &n;
			}
			else {
				Nnet n("manual.bin");
				ref = &n;
			}
			

			if (yesNoPromt("Do you want to train the Network?"))
			{//Train Network
				SizeWindow(area);
				return CONTINUE_TEXT;
			}
			else//Test Network
				return LOAD_NET;
		}
		else//New Network
		{
			LoadFile(image,text);

			SizeWindow(area);
			if (text != "")
				return NEW_TEXT;
			else if(image.data)
				return NEW_IMAGE;
			else
				return NEW_DATA;
		}
	}
}

state Menu::StartMenu(RECT * area,Nnet *&ref,problem_type pt,Mat &image,string &text)
{
	cout << "--------------------Start----------------------------" << endl;
	string version = "0.7.0";
	cout << "Solution Finder " << version << ":" << std::endl;

	return mainMenu(area,ref,pt,image,text);
}

void Menu::MenuSizeWindow(RECT * area)
{
	HWND handle;
	//string window;
	//cout << "Select name of window to watch: ";
	//cin >> window;
	Mat blank;
	namedWindow("Size Me", WINDOW_FREERATIO);
	//imshow("Size Me",blank);
	cout << "Set the size of the \"Size Me\" window and press enter";
	waitKey();

	if (handle = FindWindow(NULL, TEXT("Size Me"))) {
		//std::cout << "Found!"; 
		GetWindowRect(handle, area);
		DestroyWindow(handle);
	}
	else {std::cout << "not found";}
}

state Menu::FinishTrainMenu(RECT* area,Nnet *&ref,problem_type pt,Mat &image,string &text)
{
	cout << "--------------------END----------------------------" << endl;
	cout << "Done Generating Network" << std::endl;
	return mainMenu(area,ref,pt,image,text);
}