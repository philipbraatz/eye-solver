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

void EvoNet::DoEpoch(vector<float> truth,bool max)
{
	time_epoch = 0;

	vector<float> output;
	for (size_t i = 0; i < size; i++)
	{
		pop[i].setScore(
			GetTotalDif(truth,
			output = pop[i].Propigate(truth))
		);
		time_epoch += pop[i].GetSpeed();
	}
	time_epoch++;
	time_epoch--;
	Reorder(max);
}

void EvoNet::Reorder(bool max)
{

	for (size_t i = 0; i < size; i++)
	{
		int spot=-1;
		for (size_t j = 0; j < i; j++)
		{
			if (max)
			{
				if (pop[i].getScore() < pop[j].getScore())//if test is less/greater than anything infront it
								spot = j;//make it the best spot to be
			}
			else
			{
				if (pop[i].getScore() > pop[j].getScore())//if test is less/greater than anything infront it
					spot = j;//make it the best spot to be
			}
			
		}
		if(spot != -1)
			std::swap(pop[i], pop[spot]);//they trade places with best spot
	}
}
void EvoNet::repopulate(float save)
{
	time_repop = 0;

	int saved = save*size;
	pop.resize((int)(size*save));
	for (size_t i = 0; i < size - saved; i++)
	{
		int parent = (int)(RandNum()*saved);
		pop.push_back(pop[i]);
		pop.back().Mutate(rate);

		time_repop += pop.back().GetSpeed();
	}
	time_repop++;
	time_repop--;
}
//maximize or minimize
void EvoNet::updateStats(bool max)
{
	prevmed = average;
	if (max)
		best = -RAND_MAX;
	else
		best = RAND_MAX;
	average = 0;

	bestout = pop.front().getLastLayer();
	for (size_t i = 0; i < size; i++)
	{
		average += pop[i].getScore();
		if (max)
			if (best > pop[i].getScore())
			{
				best = pop[i].getScore();
				bestout = pop[i].getLastLayer();
			}
		else
			if (best < pop[i].getScore())
			{
				best = pop[i].getScore();
				bestout = pop[i].getLastLayer();
			}

	}
	if (best == abs(RAND_MAX))
	{
		
	}
	average /= size;
}