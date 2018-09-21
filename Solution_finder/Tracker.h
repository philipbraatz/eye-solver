#pragma once
#include <vector>
#include <iomanip>

#include "utility.h"
using namespace std;

class Tracker
{
public:
	Tracker()
	{
		prevScore.y = -RAND_MAX;
		prevScore.x = 0;
		topScore.y = -RAND_MAX;
		topScore.x = 0;
		increases.resize(10);
	}

	void display()
	{
		cout << std::setprecision(0);
		getSpeed();
		getPrevGenScore();
		getTopScore();
		if (output != "")
			cout << "\"" << output << "\"";
		if (testString != output)
			cout << endl;	
		else
			cout << "\r";

		testString = output;
	}

	void update(double gps_,int Gen,double best, string out_ ="")
	{
		output = out_;
		gps = gps_;
		prevScore.x = Gen;
		prevScore.y = best;
		if (topScore.y < prevScore.y)
			topScore = prevScore;

	}
	void getSpeed()
	{
		cout << "gens/sec: " << to_string(gps) << "\t";
	}
	void getPrevGenScore()
	{
		cout << "Gen: " << to_string(prevScore.x) << "\tScore: " << to_string(prevScore.y) << "\t";
	}
	void getTopScore()
	{
		cout << "Top Gen: " << topScore.x << "\tTop Score: " << topScore.y <<"\t";
	}
	int getIncrease(int number)
	{
		return increases[number];
	}
private:

	//previous score->previous gen# + score
	//top Nnet->gen# +score
	//10 last increases->gen#
	int gps;//generations per second
	ifPoint prevScore;//previous score->previous gen# + score
	string testString;
	ifPoint topScore;//top Nnet->gen# +score
	vector<int> increases;// increases every gen
	string output;
};

