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

	void DoEpoch(vector<float> truth);

	struct order{
		int place;
		float value;
	};

	void Reorder();

	// % of survivors to save and repopulate with
	void repopulate(float save);

	void updateStats();

	vector<float> getBest() { return bestout; }
	float getInprovement() { return median - prevmed; }

private:
	unsigned int size;
	float rate;

	vector<Nnet> pop;

	vector<float> bestin;
	vector<float> bestout;

	float prevmed;
	float best;
	float median;
};