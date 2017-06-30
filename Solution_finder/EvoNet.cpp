#include "EvoNet.h"



EvoNet::EvoNet(
	int population, float mutateRate,
	float Ninputs, float Nhiddens, int SizeHidden, float Noutputs
)
{
	size = population;
	rate = mutateRate;
	for (size_t i = 0; i < population; i++)
	{
		Nnet temp(Ninputs, Nhiddens, SizeHidden, Noutputs);
		pop.push_back(temp);
	}
}

void EvoNet::DoEpoch(vector<float> truth)
{
	vector<float> output;
	for (size_t i = 0; i < size; i++)
	{
		pop[i].setScore(
			(int)GetTotalDif(truth,
				output = pop[i].Propigate(truth)
			)
		);
	}
	Reorder();
}

void EvoNet::Reorder()
{

	for (size_t i = 0; i < size; i++)
	{
		int spot=-1;
		for (size_t j = 0; j < i; j++)
		{
			if (pop[i].getScore() < pop[j].getScore())//if test is less than anything infront it
				spot = j;//make it the best spot to be
		}
		if(spot != -1)
			std::swap(pop[i], pop[spot]);//they trade places with best spot
	}
}
void EvoNet::repopulate(float save)
{
	int saved = save*size;
	pop.resize((int)(size*save));
	for (size_t i = 0; i < size - saved; i++)
	{
		int parent = (int)(RandNum()*saved);
		pop.push_back(pop[i]);
		pop.back().Mutate(rate);
	}
}

void EvoNet::updateStats()
{
	prevmed = average;
	best = 0;
	average = 0;
	for (size_t i = 0; i < size; i++)
	{
		average += pop[i].getScore();
		if (best < pop[i].getScore())
		{
			best = pop[i].getScore();
			bestout = pop[i].getLastLayer();
		}
	}
	average /= size;
}