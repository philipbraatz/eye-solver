#pragma once
#include "EvoNet.h"

template<class tnet>
inline EvoNet<tnet>::EvoNet(
	int population, double mutateRate,
	double Ninputs, double Nhiddens, int SizeHidden, double Noutputs,
	problem_type pt
)
{
	genCount = 0;
	size = population;
	rate = mutateRate;
	for (auto i = 0; i < population; i++)
	{
		tnet temp(Ninputs, Nhiddens, SizeHidden, Noutputs,pt);
		pop.push_back(temp);
	}
}

//lilNet ONLY
template<class tnet>
inline void EvoNet<tnet>::PruneAll()
{
	for (size_t i = 0; i < size; i++)
	{
		//pop[i].StartPrune();//tempDisabled
	}
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
	//#pragma loop(hint_parallel(8)) 
	for (auto i = 0; i < size; i++)//go through whole population
	{
		if (false)
		{//TODO better thread implentation
			if (maxThreads >= t.size())//limit threads
			{
				for (auto i = 0; i < t.size(); i++)//wait for all threads to finish
				{
					t.back()->join();
					t.pop_back();//remove thread
				}
			}
			//add new threads
			if(pt == TEXT)
				t.push_back(new std::thread([=] {SingleEpocTxt(output,i,input, testing, max,prune); }));
			else if (pt == IMAGE)
				t.push_back(new std::thread([=] {SingleEpocImg(output, i, input, testing, max, prune); }));
		}
		else//works better IDK why
		{
			if (pt == TEXT)
				SingleEpocTxt(output, i, input, testing, max, prune);
			else if (pt == IMAGE)
				SingleEpocImg(output, i, input, testing, max, prune);
			else
				std::cout << "Problem Type NOT specified";
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

	for (auto i = 0; i < size; i++)
	{
		int spot=-1;
		for (auto j = 0; j < i; j++)
		{
			if (max)
				if (pop[i].getScore() > pop[j].getScore())//if i is greater than j
								spot = j;//make it the best spot to be
			else
				if (pop[i].getScore() < pop[j].getScore())//if i is smaller than j
					spot = j;//make it the best spot to be
			
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
	for (auto i = 0; i < size - saved; i++)
	{
		int parent = (int)(RandNum()*(saved-1));
		pop.push_back(pop[parent]);
		pop.back().Mutate(rate);

		time_repop += pop.back().GetSpeed();
	}
}




template<class tnet>
void EvoNet<tnet>::SingleEpocTxt(vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune)
{
	{
		if (!prune)
			output = pop[i].Propigate(input);
		else
		{
			//output = pop[i].PropPrune(input);//temp dissabled
		}

		double score = 0;
		for (auto j = 0; j < input.size(); j++)
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
		//for (auto j = 0; j < truth.size(); j++)
		//{
		//	score += pow(abs(output[j] - truth[j])*truth.size(),2);
		//}
		pop[i].setScore(score);

		time_epoch += pop[i].GetSpeed();
	}
}
template<class tnet>
void EvoNet<tnet>::SingleEpocImg(vector<double> output, int i, vector<double> input, bool testing, bool max, bool prune)
{
	//pop[i].setScore(
	//	GetTotalDif(truth,
	//	output = pop[i].Propigate(truth))
	//);
	if (!prune)
		output = pop[i].Propigate(input);
	/*else
	{
		output = pop[i].PropPrune(input);
	}*/

	double score = 0;
	for (unsigned int j = 0; j < input.size(); j++)
	{
		if ((int)(output[j] * 128) == (int)(input[j] * 128))
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

	bestout[genCount-1] = pop.front().getLastLayer();//start with last network as best
	for (auto i = 0; i < size; i++)
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
			std::cout << "No Improvement";
		if (average > prevmed)
			std::cout << "No Improvement";
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
	for (auto i = 0; i < size - 1; i++)
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