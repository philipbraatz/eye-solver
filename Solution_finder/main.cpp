#include <ctime>

#include <string>
#include <iostream>
#include <conio.h>

#include <vector>
#include <algorithm>

#include "Nnet.h"
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

	void DoEpoch(vector<float> inputs)
	{
		for (size_t i = 0; i < size; i++)
		{
			pop[i].setScore(
				GetTotalDif(inputs,
					pop[i].Propigate(inputs)
				)
			);
		}
	}



	struct order
	{
		int place;
		float value;
	};

	void Rank()
	{
		for (size_t i = 0; i < size; i++)
		{
			order temp;
			temp.place = i;
			temp.value = pop[i].getScore();
			rank.push_back(temp);//gets next score
			for (size_t j = 0; j < i; j++)
			{
				if (rank.back().value<rank[j].value)//if next score is less than any other score
				{
					rank.back().place = rank[j].place -1;//move it to where the next smallest score is
				}
			}
		}
	}

private:
	unsigned int size;
	float rate;

	vector<Nnet> pop;
	vector<order> rank;//lower place = lower value
};

int main()
{
	//pre-initialization
	srand(static_cast <unsigned> (time(0)));

	std::string answer = "It's Alive!";
	vector<float> input, output;
	for (size_t i = 0; i < answer.length(); i++){
		input.push_back((int)answer[i]/65.0);
		std::cout << (char)(input[i]*65);
	}
	std::cout <<std::endl;
	
	//neural network
	Nnet test(answer.length(),2,8,answer.length());
	output =test.Propigate(input);

	//show output
	for (size_t i = 0; i < output.size(); i++) {
		std::cout << (char)(output[i]*65);
	}
	std::cout << std::endl;
	
	//test.saveNet("first");

	_getch();
	return 0;
}