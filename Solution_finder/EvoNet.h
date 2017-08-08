#pragma once

#include <vector>

#include "Nnet.h"
#include "utility.h"

using std::vector;

class EvoNet
{
public:
	EvoNet(
		int population, double mutateRate,
		double Ninputs, double Nhiddens, int SizeHidden, double Noutputs
	);

	void DoEpoch(vector<double> truth,bool testing,bool max);

	struct order{
		int place;
		double value;
	};

	//vector<Nnet> getPop() { return pop; }

	void Reorder(bool max);

	// % of survivors to save and repopulate with
	void repopulate(double save);

	void inbreed(Nnet parent);

	void updateStats(bool max);

	unsigned int getGenCount() { return genCount; }

	vector<double> getPreviousBestOut()
	{ 
		if (genCount >= 2)
		{
		return bestout[genCount-2];
		}
		else
		{
			return bestout.front();
		}
	}
	vector<double> getCurrentBestOut() 
	{ return bestout[genCount-1]; }
	double getBestScore() { return best; }
	double getAveScore() { return average; }

	int getInputSize() { return pop.front().GetInputSize(); }
	int getOutputSize() { return pop.front().GetOutputSize(); }

	//in seconds
	//call after epoch and repopulation
	double getTime() { return time_epoch+time_repop; }

	void SaveBest(string name);

private:
	int size;
	double rate;

	unsigned int genCount;

	double time_epoch,time_repop;

	vector<Nnet> pop;

	vector<vector<double>> bestin, bestout;//history of all bests
	int bestNet;

	double prevmed,best,average;
};