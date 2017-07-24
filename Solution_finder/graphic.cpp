#include "graphic.h"

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

Graph::Graph(char name[], int width, int height)
{
	m_width = width; m_height = height;
	m_name = name;
	image = Mat::zeros(m_width, m_height, CV_8UC3);

	SetWindow(m_name, image, 0, 0);

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
	ScaleData();

	outData.clear();//TEST THIS
	for (size_t i = 0; i < lines.size(); i++)
	{
		vector<fPoint> line;
		for (size_t j = 0; j < lines[i].scaleData.size(); j++)
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
		Rect(xmin*xscale + space, ymin*yscale + space, xmax*xscale - space, ymax*yscale - space),
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
				1,
				8
			);
		}
	}
	imshow(m_name, image);
	waitKey(1);
}

void Graph::ScaleData()
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
	xscale = (m_width - space * 2) / xdiff / 1.5;
	yscale = (m_height - space * 2) / ydiff*1.5;
	//xscale = m_width / xmax / 1.5;
	//yscale= m_height / ymax * 1.5;

	//scale data
	for (size_t i = 0; i < lines.size(); i++)
	{
		lines[i].scaleData.resize(lines[i].rawData.size());
		for (size_t j = 0; j < lines[i].rawData.size(); j++)
		{
			fPoint scaled;
			scaled.x = (lines[i].rawData[j].x - xmin)*xscale + space;
			scaled.y = (lines[i].rawData[j].y - xmin)*yscale + space;
			lines[i].scaleData[j] = scaled;
		}
	}
}