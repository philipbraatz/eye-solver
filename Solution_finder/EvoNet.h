#pragma once
#include <thread>
#include <vector>

#include "Nnet.h"
#include "lilNet.h"
#include "utility.h"

using std::vector;

template<class tnet>
class EvoNet
{
public:
	EvoNet(
		int population, double mutateRate,
		double Ninputs, double Nhiddens, int SizeHidden, double Noutputs,
		problem_type pt
	);
	void SingleEpocTxt(vector<double> output, int i,vector<double> input, bool testing, bool max,bool prune);

	//void SingleEpocTxt(lilNet net_pop,vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune);
	
	void SingleEpocImg(vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune);
	void DoEpoch(vector<double> truth,bool testing,bool max,bool prune);

	void EvoNet<tnet>::PruneAll();

	struct order{
		int place;
		double value;
	};

	//vector<Nnet> getPop() { return pop; }

	void Reorder(bool max);

	// % of survivors to save and repopulate with
	void repopulate(double save);

	void inbreed(tnet parent);

	void updateStats(bool max);

	int getGenCount() { return genCount; }

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
	void EvoNet::LoadNet(string filename);

	void clearNetworks();

	double EvoNet<tnet>::getGeneticDiversity();

protected:
	int size;
	double rate;

	int genCount;

	double time_epoch, time_repop;

	problem_type pt =IMAGE;

	vector<tnet> pop;

	vector<vector<double>> bestin, bestout;//history of all bests
	int bestNet;

	double prevmed,best,average;
};