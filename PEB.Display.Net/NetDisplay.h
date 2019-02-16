#pragma once
#include "../PEB.Screen_Net.Net/PEB.Screen_Net.Net/Nnet.h"
#include "../PEB.Display.Graph/Screen.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;


//template<class tNet>
class NetDisplay :
	public Nnet
{
public:
	//Use this then = to Nnet
	//then add screen

	NetDisplay();
	NetDisplay(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs, problem_type mt);
	NetDisplay(std::string filename);
	NetDisplay(Screen view)
	{
		scr = view;
	}
	NetDisplay(Screen view,Nnet* net)
	{
		scr = view;
	}

	NetDisplay* operator =(Nnet *n)
	{
		return static_cast<NetDisplay*>(n);
	}

	void setScreen(Screen view)
	{
		scr = view;
	}

	//template<class Point2f>
	void DrawNetwork()
	{
		if (
			Nnet::GetLayerSize(Layer::HIDDENLCOUNT) == 0 ||
			Nnet::GetLayerSize(Layer::INPUTL) == 0 ||
			Nnet::GetLayerSize(Layer::OUTPUTL) == 0 ||
			Nnet::GetLayerSize(Layer::HIDDEN_NODESL) == 0
			)
				throw new std::exception("Network layers cannot be empty");

		const int BORDERWIDTH= 5;

		double nodeSWidth = (scr.width - BORDERWIDTH )/ (Nnet::GetLayerSize(Layer::HIDDENLCOUNT) + 2);
		double nodeSInput = (scr.height- BORDERWIDTH )/( Nnet::GetLayerSize(Layer::INPUTL));
		double nodeSOutput = (scr.height - BORDERWIDTH) / (Nnet::GetLayerSize(Layer::OUTPUTL));
		double nodeSHidden = (scr.height - BORDERWIDTH) / (Nnet::GetLayerSize(Layer::HIDDEN_NODESL));
		

		//GetGraph();
		scr.image = Mat::zeros(scr.width, scr.height, CV_8UC3);

		rectangle(
			scr.image,
			Rect(BORDERWIDTH, BORDERWIDTH, scr.width - BORDERWIDTH, scr.height - BORDERWIDTH),
			Scalar(255, 0, 0)
		);

		for (int i = 0; i < Nnet::GetLayerSize(Layer::HIDDENLCOUNT); i++)
		{
			for (int j = 1 + 1; j < Nnet::GetLayerSize(Layer::HIDDEN_NODESL); j++)
			{
				circle(
					scr.image,
					Point(i*nodeSWidth,j*nodeSHidden),
					(int)nodeSWidth/8,
					Scalar(
						Nnet::hidden[i].neurons[j].bias,
						Nnet::hidden[i].neurons[j].value,
						54
					),
					-1
				);

				/*line(
					image,
					{ (int)outData[i][j - 1].x,(int)outData[i][j - 1].y },
					{ (int)outData[i][j].x,(int)outData[i][j].y },
					lines[i].color,
					4,
					8
				);*/
			}
	 	}
		try
		{
			imshow(scr.name, scr.image);
		}
		catch (const std::exception& e)
		{
			throw new std::exception("ERROR: Network could not be displayed");
		}

		waitKey(1);
	}

private:
	Screen scr;
};

