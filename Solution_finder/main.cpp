#pragma once
#include <ctime>

#include <iomanip>

#include <string>
#include <iostream>
#include <conio.h>

#include <vector>
#include <algorithm>

#include "EvoNet.h"
#include "Nnet.h"
#include "utility.h"
using std::vector;



int main()
{
	//pre-initialization
	srand(static_cast <unsigned> (time(0)));

	int start = 33;
	int end=126;
	std::string answer = "It's Alive!";
	vector<float> input, output;
	for (size_t i = 0; i < answer.length(); i++){
		input.push_back(((int)answer[i]- start)/(end - start));
		std::cout << (char)((input[i]- start)/(end - start));
	}
	std::cout <<std::endl;
	

	EvoNet group(100, .05, answer.length(), 2, 8, answer.length());

	int count =0;
	vector<float> sbest;

	//MAIN LOOP
	while (true)
	{
		group.DoEpoch(input);
		group.repopulate(.2);
		group.updateStats();

		sbest =group.getBest();
		for (size_t i = 0; i < answer.length(); i++)
		{
			std::cout << (char)(sbest[i] * (end - start) + start);
		}

		std::cout << std::fixed << std::setprecision(16);
		std::cout << std::endl;



		count++;
		//_getch();
	}
	_getch();
	return 0;
}