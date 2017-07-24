#pragma once

#include <vector>

#include "Nnet.h"
#include "utility.h"

using std::vector;

class EvoNet
{
public:
	EvoNet(
		int population, float mutateRate,
		float Ninputs, float Nhiddens, int SizeHidden, float Noutputs
	);

	void DoEpoch(vector<float> truth,bool max);

	struct order{
		int place;
		float value;
	};

	void Reorder(bool max);

	// % of survivors to save and repopulate with
	void repopulate(float save);

	void updateStats(bool max);

	unsigned int getGenCount() { return genCount; }

	vector<vector<float>> getAllBestOut() { return bestout; }
	vector<float> getCurrentBestOut() 
	{ return bestout[genCount-1]; }
	float getBestScore() { return best; }
	float getAveScore() { return average; }

	//in seconds
	//call after epoch and repopulation
	double getTime() { return time_epoch+time_repop; }

private:
	int size;
	float rate;

	unsigned int genCount;

	double time_epoch,time_repop;

	vector<Nnet> pop;

	vector<vector<float>> bestin, bestout;//history of all bests

	float prevmed,best,average;
};