#pragma once
#include "Graph.h"

//shows image under name at x,y. resized to fit the image
//call once to make the window
int SetWindow(char name[], Mat Img, int x, int y)
{
	if (Img.empty())// Check for invalid input
		return -1;
	else
	{
		namedWindow(name, CV_WINDOW_KEEPRATIO);
		resizeWindow(name, Img.rows, Img.cols);
		moveWindow(name, x, y);
		imshow(name, Img);

		return 0;
	}
}

//template<class Point2f>
Graph::Graph(char name[], int width, int height)
{
	Setup(name, width, height);
}

//template<class Point2f>
void Graph::Setup(char name[], int width, int height)
{
	Screen n;
	if (name != n.name)
	{
		m_width = width; m_height = height;
		m_name = name;
		image = Mat::zeros(m_width, m_height, CV_8UC3);

		SetWindow(m_name, image, 0, 0);
	}
	else
	{
		throw new exception("ERROR: setup invalid");
	}
}

//template<class Point2f>
int Graph::AddLine(Scalar color)
{
	linedata newline;
	newline.color = color;
	lines.push_back(newline);
	return lines.size();//return lines id
}

//template<class Point2f>
int Graph::AddLine(Point2f Point, Scalar color)
{
	linedata newline;
	newline.color = color;
	newline.rawData.push_back(Point);
	lines.push_back(newline);
	return lines.size();//return lines id
}

//template<class Point2f>
int Graph::AddLine(Point2f a, Point2f b, Scalar color)
{
	linedata newline;
	newline.color= color;
	newline.rawData.push_back(a);
	newline.rawData.push_back(b);
	lines.push_back(newline);
	return lines.size();//return lines id
}

//template<class Point2f>
int Graph::AddLine(vector<Point2f> alldata, Scalar color) {
	linedata newline;
	newline.color = color;
	newline.rawData = alldata;
	lines.push_back(newline);
	return lines.size();//return lines id
}

//template<class Point2f>
void Graph::DeleteLine(int lineID)
{
	lines.erase(lines.begin()+lineID-1);
}

//template<class Point2f>
void Graph::DeletePoint(int lineID, bool fromLast, int count)
{
	if(fromLast)
		for (int i = 0; i < count; i++) lines[lineID].rawData.pop_back();
	else
		lines[lineID].rawData.erase(lines[lineID].rawData.begin(), lines[lineID].rawData.begin()+count);
}

//template<class Point2f>
vector<vector<Point2f>> Graph::GetGraph()
{
	//CompactData();//uncommented

	outData.clear();

	for (auto i = 0; i < lines.size(); i++)
	{
		outData.push_back(ScaleLine(i));
	}
	return outData;
}

//template<class Point2f>
void Graph::DrawGraph()
{
	GetGraph();
	image = Mat::zeros(m_width, m_height, CV_8UC3);

	rectangle(
		image,
		Rect(space, space, (xmax - xmin)*xscale - space, (ymax - ymin)*yscale - space),
		Scalar(255, 0, 0)
	);

	for (int i = 0; i < outData.size(); i++)
	{
		for (int j = 1 + 1; j < outData[i].size(); j++)
		{
			line(
				image,
				{ (int)outData[i][j - 1].x,(int)outData[i][j - 1].y },
				{ (int)outData[i][j].x,(int)outData[i][j].y },
				lines[i].color,
				4,
				8
			);
		}
	}
	try
	{
		//cout << m_name;
		imshow(m_name, image);
	}
	catch (const std::exception&)
	{
		throw new exception("ERROR: Graph could not be displayed");
	}

	waitKey(1);
}

//template<class Point2f>
void Graph::CompactData()//Broken
{
	vector<linedata> nlines;
	for (auto i = 0; i < lines.size(); i++)
		if (lines[i].rawData.size() > COMPACT_SIZE)
			for (auto j = 0; j < lines[j].rawData.size() - 2; j += 2)
				nlines[i].rawData.push_back(lines[i].rawData[j]);
	//return nlines;
}

//template<class Point2f>
vector<Point2f> Graph::ScaleLine(int line)
{
	vector<Point2f> scaledData;

	//get data lengths

	xmax = -RAND_MAX;
	ymax = -RAND_MAX;
	xmin = RAND_MAX;
	ymin = RAND_MAX;
	for (auto i = 0; i < lines.size(); i++)
	{
		for (auto j = 0; j < lines[i].rawData.size(); j++)
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
	double xdiff = xmax - xmin;
	double ydiff = ymax - ymin;
	xscale = (m_width - space * 2) / xdiff / 1.5;
	yscale = (m_height - space * 2) / ydiff * 1.5;

	//scale data
	scaledData.resize(lines[line].rawData.size());
	for (auto j = 0; j < lines[line].rawData.size(); j++)
	{
		Point2f scaled;
		scaled.x = (lines[line].rawData[j].x - xmin)*xscale + space;
		scaled.y = (lines[line].rawData[j].y - xmin)*yscale + space;
		scaledData[j] = scaled;
	}
	return scaledData;
}