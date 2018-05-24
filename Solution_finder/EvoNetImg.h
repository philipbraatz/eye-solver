#pragma once
#include "EvoNet.h"
template<class tnet>
class EvoNetImg :
	public EvoNet<tnet>
{
public:
	void SingleEpoc(vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune)
	{
		//pop[i].setScore(
		//	GetTotalDif(truth,
		//	output = pop[i].Propigate(truth))
		//);
		if (!prune)
			output = pop[i].Propigate(input);
		else
			output = pop[i].PropPrune(input);

		double score = 0;
		for (unsigned int j = 0; j < input.size(); j++)
		{
			if ((int)(output[j]*128) == (int)(input[j]*128))
			{
				score++;
			}
			else
			{
				score -= pow(abs(output[j] - input[j]), 2);
			}
		}
		pop[i].setScore(score);

		time_epoch += pop[i].GetSpeed();
	}
};

