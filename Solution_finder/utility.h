#pragma once
#include <cstdlib>

static float RandNum() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }

static float GetTotalDif(vector<float> input, vector<float> output)
{
	float total = 0;
	for (size_t i = 0; i < input.size(); i++)
		total += abs(input[i] - output[i]);
	return total;
}