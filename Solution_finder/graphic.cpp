#pragma once
#include "graphic.h"

//shows image under name at x,y. resized to fit the image
//call once to make the window
int SetWindow(char name[] ,Mat Img, int x, int y)
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

Graph::Graph(char name[], int width, int height)
{
	Setup(name, width, height);
}

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
		cout << "ERROR setup invalid";
	}
}

int Graph::AddLine(vector<fPoint> alldata) {
	linedata newline;
	newline.rawData = alldata;
	lines.push_back(newline);
	colors.push_back(Scalar(RandNum() * 255, RandNum() * 255, RandNum() * 255));
	return lines.size();//return lines id
}

vector<vector<fPoint>> Graph::GetGraph()
{
	//CompactData();//uncommented
	ScaleData();

	outData.clear();
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		vector<fPoint> line;
		for (unsigned int j = 0; j < lines[i].scaleData.size(); j++)
			line.push_back(lines[i].scaleData[j]);
		outData.push_back(line);
	}
	return outData;
}

void Graph::DrawGraph()
{
	GetGraph();
	image = Mat::zeros(m_width, m_height, CV_8UC3);

	rectangle(
		image,
		Rect( space, space, (xmax-xmin)*xscale - space, (ymax-ymin)*yscale - space),
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
				colors[i],
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
		cout << "ERROR: Image Invalid" << endl;
	}

	waitKey(1);
}

void Graph::CompactData()//Broken
{
	vector<linedata> nlines;
	for (unsigned int i = 0; i < lines.size(); i++)
		if (lines[i].rawData.size() > COMPACT_SIZE)
			for (unsigned int j = 0; j < lines[j].rawData.size() - 2; j += 2)
				nlines[i].rawData.push_back(lines[i].rawData[j]);
	//return nlines;
}

void Graph::ScaleData()
{
	//get data lengths

	xmax = -RAND_MAX;
	ymax = -RAND_MAX;
	xmin = RAND_MAX;
	ymin = RAND_MAX;
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		for (unsigned int j = 0; j < lines[i].scaleData.size(); j++)
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
	yscale = (m_height - space * 2) / ydiff*1.5;
	//xscale = m_width / xmax / 1.5;
	//yscale= m_height / ymax * 1.5;

	//scale data
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		lines[i].scaleData.resize(lines[i].rawData.size());
		for (unsigned int j = 0; j < lines[i].rawData.size(); j++)
		{
			fPoint scaled;
			scaled.x = (lines[i].rawData[j].x - xmin)*xscale + space;
			scaled.y = (lines[i].rawData[j].y - xmin)*yscale + space;
			lines[i].scaleData[j] = scaled;
		}
	}
}