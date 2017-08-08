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
	double value;
	vector<double> weights;
	double bias;

	vector<double> deltavalues;
	neuron() {};
};
struct layer
{
	vector<neuron> neurons;
	unsigned int size;
};

class Nnet
{
public:

	vector<double> m_goal;

	Nnet();
	Nnet(double Ninputs, double Nhiddens, int SizeHidden, double Noutputs);
	Nnet(std::string filename);
	//Nnet(double Ninputs, double Nhiddens, int SizeHidden, double Noutputs, std::vector<double> goal);

	void Setup(double Ninputs, double Nhiddens, int SizeHidden, double Noutputs, bool loaded = false);
	unsigned int GetLayerSize(type l);

	//set input and get output
	vector<double> Propigate(vector<double> inputs);

	unsigned int getAge() { return age; }

	void setScore(double score) { m_score = score; }
	double getScore() { return m_score; }

	void Mutate(double rate);

	vector<double> Nnet::getLastLayer();

	//in seconds
	//call after propigate and mutate
	double GetSpeed();

	void saveNet(std::string name);
	void loadNet(std::string filename);

	int GetInputSize() { return input.size; }
	int GetOutputSize() { return output.size; }

private:
	//const int MAX_WEIGHT = 9999;

	double m_score;

	int age;

	clock_t startProp,startMut;
	double PassedProp,PassedMut;

	//layers
	layer input;
	layer output;
	vector<layer> hidden;

	unsigned int m_Nhidden;

	void Normalize(double &input);
	void AddBiases(vector<double> cur, vector<double> biases, vector<double> &out);

	void MutTable(double &weight);
};