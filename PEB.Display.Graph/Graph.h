#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//#include <opencv2/core/types.hpp>

#include <vector>

//#include "../PEB.Utility/Utility.h"

using namespace cv;
using namespace std;

const int COMPACT_SIZE = 10000;//CRASHES

//template<class Point2f =Point2f>
struct linedata
{
	vector<Point2f> rawData;
	Scalar color;
	//vector<fPoint2f> scaleData;
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

///<summary>
///shows image under name at x,y. resized to fit the image
///call once to make the window
///</summary>
int SetWindow(char name[], Mat Img, int x, int y);

//template<class Point2f>
class Graph
{
public:
	Graph() {}
	Graph(char name[], int width, int height);

	void Setup(char name[], int width, int height);

	void SetLocation(int x, int y) { moveWindow(m_name, x, y); }

	//autoajust: bool
	//	scales graph based on data
	//zero: bool
	//	view only above zero
	//lineLength: int
	//	deletes data past set length (moving graph)

	int AddLine(Scalar color = {0,0,0});
	int AddLine(Point2f Point, Scalar color);
	int AddLine(Point2f a, Point2f b, Scalar color);//adds a 2 Point2f line
	int AddLine(vector<Point2f> allLine, Scalar color);

	void DeleteLine(int lineID);
	void DeleteAllLines() { lines.clear(); }

	void AddPoint(Point2f Point, int lineID){lines[lineID].rawData.push_back(Point);}
	void AddPointsList(vector<Point2f> list, int lineID) {for (int i = 0; i < list.size(); i++){AddPoint(list[i],lineID);}}

	void DeletePoint(int lineID, bool fromLast, int count);

	vector<vector<Point2f>> GetGraph();

	void DrawGraph();

private:
	double xmin, xmax, ymin, ymax, xscale, yscale;
	int m_width, m_height;

	const int space = 5;

	vector<linedata> lines;
	//vector<Scalar> colors;
	vector<vector<Point2f>> outData;
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

	vector<Point2f> ScaleLine(int line);

	void CompactData();
};
