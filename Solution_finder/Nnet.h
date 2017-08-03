#pragma once
#include <vector>

#include <string>//Save/Load file
#include <fstream>
#include <iostream>//DEBUG

//timer
#include <ctime>
#include <cstdlib>

using std::vector;

const double e = 2.71828182845904523536;

enum type {
	Ninputs, Nhiddens, Noutputs, SizeHidden
};

struct neuron
{
	float value;
	vector<float> weights;
	float bias;

	vector<float> deltavalues;
	neuron() {};
};
struct layer
{
	vector<neuron> neurons;
	int size;
};

class Nnet
{
public:

	vector<float> m_goal;

	Nnet();
	Nnet(float Ninputs, float Nhiddens, int SizeHidden, float Noutputs);
	Nnet(std::string filename);
	//Nnet(float Ninputs, float Nhiddens, int SizeHidden, float Noutputs, std::vector<float> goal);

	void Setup(float Ninputs, float Nhiddens, int SizeHidden, float Noutputs, bool loaded = false);
	unsigned int GetLayerSize(type l);

	//set input and get output
	vector<float> Propigate(vector<float> inputs);

	unsigned int getAge() { return age; }

	void setScore(float score) { m_score = score; }
	float getScore() { return m_score; }

	void Mutate(double rate);

	vector<float> Nnet::getLastLayer();

	//in seconds
	//call after propigate and mutate
	double GetSpeed();

	void saveNet(std::string name);
	void loadNet(std::string filename);

	int GetInputSize() { return input.size; }
	int GetOutputSize() { return output.size; }

private:
	//const int MAX_WEIGHT = 9999;

	float m_score;

	int age;

	clock_t startProp,startMut;
	double PassedProp,PassedMut;

	//layers
	layer input;
	layer output;
	vector<layer> hidden;

	unsigned int m_Nhidden;

	void Normalize(float &input);
	void AddBiases(vector<float> cur, vector<float> biases, vector<float> &out);

	void MutTable(float &weight);
};