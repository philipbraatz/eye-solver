#pragma once
#include <ctime>

#include <iomanip>

#include <string>
#include <iostream>
#include <conio.h>

#include <vector>
#include <algorithm>

#include "Nnet.h"
#include "utility.h"
using std::vector;


float GetTotalDif(vector<float> input, vector<float> output)
{
	float total=0;
	for (size_t i = 0; i < input.size(); i++)
		total += abs(input[i] - output[i]);
	return total;
}

class EvoNet
{
public:
	EvoNet(
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

	void DoEpoch(vector<float> truth)
	{
		vector<float> output;
		for (size_t i = 0; i < size; i++)
		{
			pop[i].setScore(
				GetTotalDif(truth,
					output=pop[i].Propigate(truth)
				)
			);
			//for (size_t i = 0; i < truth.size(); i++) {
			//	std::cout << (char)(output[i]*(126-33)+33);
			//}
			//std::cout <<" : "<<pop[i].getScore() << std::endl;//DEBUG
		}
		Reorder();
	}



	struct order
	{
		int place;
		float value;
	};

	void Reorder()
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
	// % of survivors to save and repopulate with
	void repopulate(float save)
	{
		int saved = save*size;
		pop.resize((int)(size*save));
		for (size_t i = 0; i < size-saved; i++)
		{
			int parent = (int)(RandNum()*saved);
			pop.push_back(pop[i]);
			pop.back().Mutate(rate);
		}
	}

	void updateStats()
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

	vector<float> getbest() { return bestout; }
	float getInprovement() {return median - prevmed; }

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

int main()
{
	//pre-initialization
	srand(static_cast <unsigned> (time(0)));

	int start = 33;
	int end=126;
	std::string answer = "It's Alive!";
	vector<float> input, output;
	for (size_t i = 0; i < answer.length(); i++){
		input.push_back(((int)answer[i]-33)/(126-33));
		std::cout << (char)((input[i]-33)/(126-33));
	}
	std::cout <<std::endl;
	

	EvoNet group(100, .05, answer.length(), 2, 8, answer.length());

	int count =0;
	std::string sbest;

	//MAIN LOOP
	while (true)
	{
		group.DoEpoch(input);
		group.repopulate(.2);
		group.updateStats();

		for (size_t i = 0; i < answer.length(); i++)
		{
			
		}

		std::cout << std::fixed << std::setprecision(5);
		std::cout <<"..."<<group.getInprovement() << std::endl;



		count++;
		//_getch();
	}
	_getch();
	return 0;
}