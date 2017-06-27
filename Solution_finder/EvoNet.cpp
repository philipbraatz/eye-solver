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
			GetTotalDif(truth,
				output = pop[i].Propigate(truth)
			)
		);
		//for (size_t i = 0; i < truth.size(); i++) {
		//	std::cout << (char)(output[i]*(126-33)+33);
		//}
		//std::cout <<" : "<<pop[i].getScore() << std::endl;//DEBUG
	}
	Reorder();
}

void EvoNet::Reorder()
{

	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < i; j++)
		{
			if (pop[i].getScore() < pop[j].getScore())//if test is less than anything infront it
			{
				std::swap(pop[i], pop[j]);//they trade places
			}
		}

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
	prevmed = median;
	best = 0;
	median = 0;
	for (size_t i = 0; i < size; i++)
	{
		median += pop[i].getScore();
		if (best < pop[i].getScore())
		{
			best = pop[i].getScore();
			bestout = pop[i].getLastLayer();
		}
	}
	median /= size;
}