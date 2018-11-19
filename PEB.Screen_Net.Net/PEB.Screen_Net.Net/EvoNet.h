#pragma once
#include <thread>
#include <vector>
#include <string>

#include "Nnet.h"
#include "lilNet.h"
//#include "../PEB.Utility/Utility.h"

using std::vector;

template<class tnet>
class EvoNet
{
public:
	int counter;//temp

	EvoNet(
		int population, double mutateRate,
		double Ninputs, double Nhiddens, int SizeHidden, double Noutputs,
		problem_type pt
	);
	void SingleEpocTxt(vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune);

	//void SingleEpocTxt(lilNet net_pop,vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune);

	void SingleEpocImg(vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune);
	void DoEpoch(vector<double> truth, bool testing, bool max, bool prune);

	void EvoNet<tnet>::PruneAll();

	struct order {
		int place;
		double value;
	};

	//vector<Nnet> getPop() { return pop; }

	void Reorder(bool max);

	void change_mutateRate(double _rate);

	// % of survivors to save and repopulate with
	void repopulate(double save);

	void inbreed(tnet parent);

	void updateStats(bool max);

	int getGenCount() { return genCount; }

	vector<double> getPreviousBestOut()
	{
		if (genCount >= 2)
		{
			return bestout[genCount - 2];
		}
		else
		{
			return bestout.front();
		}
	}
	vector<double> getCurrentBestOut()
	{
		return bestout[genCount - 1];
	}
	double getBestScore() { return best; }
	double getAveScore() { return average; }
	double getSuccessRate() { return successRate; }

	int getInputSize() { return pop.front().GetInputSize(); }
	int getOutputSize() { return pop.front().GetOutputSize(); }

	//in seconds
	//call after epoch and repopulation
	double getTime() { return time_epoch + time_repop; }

	void SaveBest(std::string name);
	void EvoNet::LoadNet(std::string filename);

	void clearNetworks();

	double EvoNet<tnet>::getGeneticDiversity();

protected:
	int size;
	double rate;

	int genCount;

	double time_epoch, time_repop;

	problem_type pt = TEXT;

	vector<tnet> pop;

	vector<vector<double>> bestin, bestout;//history of all bests
	int bestNet;

	double prevmed, best, average;

	double successRate, prevBest;
};