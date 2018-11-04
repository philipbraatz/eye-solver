#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//#include <opencv2/core/types.hpp>

#include <vector>

#include <utility.h>

using namespace cv;
using namespace std;

const int COMPACT_SIZE = 10000;//CRASHES

struct Slider
{
	double value;
	int slider_value;
	int max;
};

struct linedata
{
	vector<fPoint> rawData;
	vector<fPoint> sortData;
	vector<fPoint> scaleData;
};

struct Screen
{
	Mat image;
	char* name;
	int width;
	int height;
	int x;
	int y;
};

//shows image under name at x,y. resized to fit the image
//call once to make the window
int SetWindow(char name[], Mat Img, int x, int y);

class Graph
{
public:
	Graph() {}
	Graph(char name[], int width, int height);

	void Setup(char name[], int width, int height);
	
	void SetLocation(int x, int y){ moveWindow(m_name, x, y); }

	int AddLine(vector<fPoint> alldata);
	void AddData(fPoint p, int id)
	{
		lines[id].rawData.push_back(p);
	}

	vector<vector<fPoint>> GetGraph();

	void DrawGraph();

private:
	double xmin, xmax,ymin,ymax,xscale,yscale;
	int m_width, m_height;

	const int space = 5;

	vector<linedata> lines;
	vector<Scalar> colors;
	vector<vector<fPoint>> outData;
	char* m_name;

	Mat image;

	//void SortData() {
	//	for (auto i = 0; i < lines.size(); i++)//TODO sort data
	//		lines[i].sortData = lines[i].rawData;
	//}

	//{
	//	for (auto i = 0; i < lines.size(); i++)
	//	{
	//		for (auto j = 0; j < lines[i].rawData.size(); j++)
	//		{
	//			double a = lines[i].rawData[j].x;
	//
	//			for (auto k = 0; k < lines[i].rawData.size(); k++)
	//			{
	//				double b = lines[i].rawData[k].x;
	//				if (a<b && j!=k)
	//				{
	//
	//				}
	//			}
	//		}
	//	}
	//}

	void ScaleData();

	void CompactData();
};
