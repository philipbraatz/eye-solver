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

	vector<float> getBestOut() { return bestout; }
	float getBestScore() { return best; }
	float getAveScore() { return average; }

	//in seconds
	//call after epoch and repopulation
	double getTime() { return time_epoch+time_repop; }

private:
	unsigned int size;
	float rate;

	double time_epoch,time_repop;

	vector<Nnet> pop;

	vector<float> bestin;
	vector<float> bestout;

	float prevmed;
	float best;
	float average;
};