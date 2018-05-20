#pragma once
#include "EvoNet.h"

template<class tnet>
inline EvoNet<tnet>::EvoNet(
	int population, double mutateRate,
	double Ninputs, double Nhiddens, int SizeHidden, double Noutputs
)
{
	genCount = 0;
	size = population;
	rate = mutateRate;
	for (unsigned int i = 0; i < population; i++)
	{
		tnet temp(Ninputs, Nhiddens, SizeHidden, Noutputs);
		pop.push_back(temp);
	}
}

//lilNet ONLY
template<class tnet>
inline void EvoNet<tnet>::PruneAll()
{
	for (size_t i = 0; i < size; i++)
	{
		pop[i].StartPrune();
	}
}

//should be called in thread
template<class tnet>
inline void EvoNet<tnet>::SingleEpoc(vector<double> output,int i,vector<double> input, bool testing, bool max,bool prune)
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
		if ((int)(output[j] * (126 - 32) + 32) == (int)(input[j] * (126 - 32) + 32))
		{
			score++;
		}
		else
		{
			score -= pow(abs(output[j] - input[j]), 2);
		}
	}

	//int score=0;
	//for (unsigned int j = 0; j < truth.size(); j++)
	//{
	//	score += pow(abs(output[j] - truth[j])*truth.size(),2);
	//}
	pop[i].setScore(score);
	
	time_epoch += pop[i].GetSpeed();
}

template<class tnet>
inline void EvoNet<tnet>::DoEpoch(vector<double> input,bool testing,bool max,bool prune)
{
	time_epoch = 0;

	if (!testing)
	{
		genCount++; //= pop.front().getAge();
	}

	vector<std::thread*> t;//threads
	int maxThreads =8;
	t.resize(size);// number of threads

	bestout.resize(genCount);

	vector<double> output;
	for (unsigned int i = 0; i < size; i++)//go through whole population
	{
		if (false)
		{
			if (maxThreads >= t.size())//limit threads
			{
				for (unsigned int i = 0; i < t.size(); i++)//wait for all threads to finish
				{
					t.back()->join();
					t.pop_back();//remove thread
				}
			}
			t.push_back(new std::thread([=] {SingleEpoc(output,i,input, testing, max,prune); }));//add new threads
		}
		else
		{
			SingleEpoc(output, i, input, testing, max,prune);//works better IDK why
		}
		
	}

	if (!testing)
	{
		Reorder(max);
	}

}

template<class tnet>
inline void EvoNet<tnet>::Reorder(bool max)
{

	for (unsigned int i = 0; i < size; i++)
	{
		int spot=-1;
		for (unsigned int j = 0; j < i; j++)
		{
			if (max)
			{
				if (pop[i].getScore() > pop[j].getScore())//if i is greater than j
								spot = j;//make it the best spot to be
			}
			else
			{
				if (pop[i].getScore() < pop[j].getScore())//if i is smaller than j
					spot = j;//make it the best spot to be
			}
			
		}
		if(spot != -1)//if better spot
			std::swap(pop[i], pop[spot]);//they trade places with best spot
	}
}
//saves a percent of the population to be parents and produces mutated babies
template<class tnet>
inline void EvoNet<tnet>::repopulate(double save)
{
	time_repop = 0;

	int saved = save*size;
	pop.resize((int)(size*save));
	for (unsigned int i = 0; i < size - saved; i++)
	{
		int parent = (int)(RandNum()*(saved-1));
		pop.push_back(pop[parent]);
		pop.back().Mutate(rate);

		time_repop += pop.back().GetSpeed();
	}
}
//maximize or minimize
template<class tnet>
inline void EvoNet<tnet>::updateStats(bool max)
{
	prevmed = average;
	//reset max to get new best
	if (max)
		best = RAND_MAX;
	else
		best = -RAND_MAX;
	average = 0;

	bestout[genCount-1] = pop.front().getLastLayer();//set an output as a fall back in case of error
	for (unsigned int i = 0; i < size; i++)
	{
		average += pop[i].getScore();
		if (max)
		{
			if (best > pop[i].getScore())
			{
				best = pop[i].getScore();
				bestout[genCount-1] = pop[i].getLastLayer();
			}
		}
		else
		if (best < pop[i].getScore())
		{
			best = pop[i].getScore();
			bestout[genCount-1] = pop[i].getLastLayer();
		}

	}
	if (best == abs(RAND_MAX))
	{
		best = 0;
	}
	average /= size;

	if (max)
	{
		if (average < prevmed)
			cout << "No Improvement";
		if (average > prevmed)
			cout << "No Improvement";
	}
}

//repopulate optimized to save only one chosen parent
template<class tnet>
inline void EvoNet<tnet>::inbreed(tnet parent)
{
	time_repop = 0;
	double mult = 2.5;
	rate *= mult;//multiply mutation due to inbreeding

	//int saved = 1;
	pop.clear();
	pop.push_back(parent);
	for (unsigned int i = 0; i < size - 1; i++)
	{
		//parent = (int)(RandNum());
		pop.push_back(pop[i]);
		pop.back().Mutate(rate);

		time_repop += pop.back().GetSpeed();
	}
	rate /= mult;//reset mutation for normal repop
}

template<class tnet>
inline void EvoNet<tnet>::SaveBest(string name)
{
	pop.front().saveNet(name);
}

template<class tnet>
inline void EvoNet<tnet>::LoadNet(string filename)
{
	tnet net;
	pop = { net.loadNet(filename) };
	inbreed(pop.front());
}