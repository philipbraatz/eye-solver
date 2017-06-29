#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <vector>

#include "utility.h"

using namespace cv;
using namespace std;

struct linedata
{
	vector<fPoint> rawData;
	vector<fPoint> sortData;
	vector<fPoint> scaleData;
};



//shows image under name at x,y. resized to fit the image
//call once to make the window
int SetWindow(char name[], Mat Img, int x, int y)
{
	if (Img.empty())// Check for invalid input
		return -1;
	else
	{
		namedWindow(name, CV_WINDOW_KEEPRATIO);
		resizeWindow(name, Img.cols, Img.rows);
		moveWindow(name, x, y);
		imshow(name, Img);

		return 0;
	}
}

struct Screen
{
	Mat image;
	char* name;
	int width;
	int height;
	int x;
	int y;
};

class Graph
{
public:
	Graph(char name[],int width, int height) 
	{ 
		m_width = width; m_height = height;
		m_name = name;
		image = Mat::zeros(m_width, m_height, CV_8UC3);

		SetWindow(m_name, image, 0, 0);

	}
	
	void SetLocation(int x, int y){ moveWindow(m_name, x, y); }

	int AddLine(vector<fPoint> alldata) { 
		linedata newline;
		newline.rawData = alldata;
		lines.push_back(newline);
		return lines.size();//return lines id
	}
	void AddData(fPoint p, int id){lines[id].rawData.push_back(p);}



	vector<vector<fPoint>> GetGraph()
	{
		SortData();
		ScaleData();

		outData.clear();//TEST THIS
		for(size_t i = 0; i < lines.size(); i++)
		{
			vector<fPoint> line;
			for (size_t j = 0; j < lines[i].scaleData.size(); j++)
				line.push_back(lines[i].scaleData[j]);
			outData.push_back(line);
		}
		return outData;
	}

	void DrawGraph()
	{
		GetGraph();
		image = Mat::zeros(m_width, m_height, CV_8UC3);

		//line(
		//	image,
		//	{50,100 },
		//	{ 200,700 },
		//	Scalar(125, 0, 255)
		//	);


		for (int i = 0; i < outData.size(); i++)
		{
			for (int j = 1+1; j < outData[i].size(); j++)
			{
				line(
					image,
					{(int)outData[i][j - 1].x,(int)outData[i][j - 1].y },
					{ (int)outData[i][j].x,(int)outData[i][j].y },
					Scalar(125, 0, 255)
					);
			}
		}
		imshow(m_name, image);
		waitKey(1);
	}

private:
	float xmin, xmax,ymin,ymax,xscale,yscale;
	int m_width, m_height;

	vector<linedata> lines;
	vector<vector<fPoint>> outData;
	char* m_name;

	Mat image;

	void SortData() {
		for (size_t i = 0; i < lines.size(); i++)//TODO sort data
			lines[i].sortData = lines[i].rawData;
	}
	//{
	//	for (size_t i = 0; i < lines.size(); i++)
	//	{
	//		for (size_t j = 0; j < lines[i].rawData.size(); j++)
	//		{
	//			float a = lines[i].rawData[j].x;
	//
	//			for (size_t k = 0; k < lines[i].rawData.size(); k++)
	//			{
	//				float b = lines[i].rawData[k].x;
	//				if (a<b && j!=k)
	//				{
	//
	//				}
	//			}
	//		}
	//	}
	//}

	void ScaleData()
	{
		//get data lengths

		xmax = -RAND_MAX;
		ymax = -RAND_MAX;
		xmin = RAND_MAX;
		ymin = RAND_MAX;
		for (size_t i = 0; i < lines.size(); i++)
		{
			for (size_t j = 0; j < lines[i].scaleData.size(); j++)
			{
				if (xmax < lines[i].rawData[j].x)
					xmax = lines[i].rawData[j].x;
				if (xmin > lines[i].rawData[j].x)
					xmin = lines[i].rawData[j].x;
				if (ymax < lines[i].rawData[j].y)
					ymax = lines[i].rawData[j].y;
				if (ymin > lines[i].rawData[j].y)
					ymin = lines[i].rawData[j].y;
			}

		}

		//set scales
		float xdiff = xmax - xmin;
		float ydiff = ymax - ymin;
		xscale = m_width / xdiff;
		yscale = m_height / ydiff;

		//scale data
		for (size_t i = 0; i < lines.size(); i++)
		{
			lines[i].scaleData.resize(lines[i].sortData.size());
			for (size_t j = 0; j < lines[i].sortData.size(); j++)
			{
				fPoint temp;
				temp.x = (lines[i].sortData[j].x-xmin)*xscale;
				temp.y =(lines[i].sortData[j].y-ymin)*yscale;
				lines[i].scaleData[j] = temp;
			}
		}
	}
};
