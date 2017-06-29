#pragma once
#include <cstdlib>
#include <vector>

struct fPoint
{
	float x, y;
};

static float RandNum() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }

static float GetTotalDif(std::vector<float> input, std::vector<float> output)
{
	float total = 0;
	for (size_t i = 0; i < input.size(); i++)
		total += abs(input[i] - output[i]);
	return total;
}