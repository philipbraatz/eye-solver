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
		dnaDiff = 0;
		score.y = -RAND_MAX;
		score.x = 0;
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
		getDiversity();

		if (output != "")
			cout << "\"" << output << "\"";
		if (testString != output)
		{
			cout << endl;
			prevScore = score;
		}
		else
			cout << "\r";

		testString = output;
	}

	void update(double gps_,int Gen,double best,double diversity, string out_ ="")
	{
		output = out_;
		gps = gps_;
		dnaDiff = diversity;
		score.x = Gen;
		score.y = best;
		if (topScore.y < score.y)
			topScore = score;

	}
	void getDiversity()
	{
		cout << "diff: " << to_string(dnaDiff) << "\t";
	}
	void getSpeed()
	{
		cout << "gens/sec: " << to_string(gps) << "\t";
	}
	void getPrevGenScore()
	{
		char p;
		if (score.y >prevScore.y)
			p = '+';
		else
			p = '-';
		cout << "Gen: " << to_string(score.x) << "\tScore: "<<p<<" " << to_string(score.y) << "\t";
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

	//	//10 last increases->gen#
	int gps;//generations per second
	double dnaDiff;
	ifPoint score;//score-> gen# + score
	ifPoint prevScore;//prevscore->previous new gen# + score
	string testString;
	ifPoint topScore;//top Nnet->gen# +score
	vector<int> increases;// increases every gen
	string output;
};

