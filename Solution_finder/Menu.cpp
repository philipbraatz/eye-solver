#pragma once

#include "Menu.h"


using std::cout;
using std::cin;
using namespace cv;

Menu* thisTrackbar;//newest Menu only

Menu::Menu()
{
	thisTrackbar = this;
}
//Trackbar code here
void Menu::_on_trackbar()
{
	SliderFrame.type = OTHER;
	SliderFrame.hiddens = hiddenSize.slider_value;
	SliderFrame.hsize = hiddenLayers.slider_value;
	SliderFrame.size = psize.slider_value;
	SliderFrame.rate = rate.slider_value / 1000;
}
//trackbars calls gobal trackbar
void on_trackbar(int , void *)
{
	thisTrackbar->_on_trackbar();//send to Current Menu
}

RECT* Menu::SizeWindow(RECT * area)
{
	HWND handle;
	Mat blank = Mat::zeros({100,200},1 );
	//defaul Settings
	hiddenSize.slider_value = 4;
	hiddenSize.max = 45;
	hiddenLayers.value = 16;
	hiddenLayers.max = 128;
	psize.slider_value = 512;
	psize.max = 20480;
	rate.slider_value = 075;
	rate.max = 125;
	
	blank = Mat::zeros(720, 480, CV_8UC3);//set default size
	SetWindow("Size Me", blank, 100, 100);
	cout << "Set the size of the \"Size Me\" window and press enter";
	//Nnet settings
	createTrackbar("Hidden Size", "Size Me", &hiddenSize.slider_value, hiddenSize.max, on_trackbar);
	createTrackbar("Hidden Layer Size", "Size Me", &hiddenLayers.slider_value, hiddenLayers.max, on_trackbar);
	createTrackbar("Population Size", "Size Me", &psize.slider_value, psize.max, on_trackbar);
	createTrackbar("Mutation Rate (/1000):", "Size Me", &rate.slider_value, rate.max, on_trackbar);
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
	//read file
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
		text = file.substr(0,file.length()-4);//TODO: make it read the file
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

state Menu::mainMenu(RECT * area, Nnet *&ref,NetFrame &netF_,Mat &image,string &text)
{
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
				 netF_= SliderFrame;
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

state Menu::StartMenu(RECT * area,Nnet *&ref,NetFrame &netf,Mat &image,string &text)
{
	cout << "Solution Finder " << VERSION << ":" << std::endl;
	return mainMenu(area,ref,netf,image,text);
}

void Menu::MenuSizeWindow(RECT * area)
{
	HWND handle;
	//string window;
	//cout << "Select name of window to watch: ";
	//cin >> window;
	Mat blank= imread("images\\shell.jpg");
	string sizeWindow = "Size Me";
	namedWindow(sizeWindow, WINDOW_FREERATIO);

	imshow("Size Me",blank);//Add Trackbars
	cout << "Set the size of the \"Size Me\" window and press enter";
	waitKey();

	if (handle = FindWindow(NULL, TEXT("Size Me"))) {
		//std::cout << "Found!"; 
		GetWindowRect(handle, area);
		DestroyWindow(handle);
	}
	else {std::cout << "not found";}
}

state Menu::FinishTrainMenu(RECT* area,Nnet *&ref, NetFrame &netf,Mat &image,string &text)
{
	cout << "Done Generating Network" << std::endl;
	return mainMenu(area,ref,netf,image,text);
}
