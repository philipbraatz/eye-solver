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

enum Layer {
	INPUTL, HIDDENL, OUTPUTL, HIDDEN_NODESL
};
enum problem_type {
	TEXT,
	IMAGE,
	VIDEO,
	OTHER
};

//FOR MENU move elsewhere if able
struct NetFrame
{
	int input, output, size, hiddens, hsize;
	double rate;
	/*string textAnswer;
	Mat imgAnswer;*/
	problem_type type;
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
	int size;
};

class Nnet
{
public:

	vector<double> m_goal;

	Nnet();
	Nnet(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs, problem_type mt);
	Nnet(std::string filename);
	//Nnet(double Ninputs, double Nhiddens, int SizeHidden, double Noutputs, std::vector<double> goal);

	int Setup(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs, problem_type mt, bool loaded = false);
	int GetLayerSize(Layer l);

	//set input and get output
	vector<double> Initial_Propigation(vector<double> input);//for inheritience
	vector<double> Final_Propigation(vector<double> input);//for inheritience
	vector<double> Propigate(vector<double> inputs);

	int getAge() { return age; }

	void setScore(double score) { m_score = score; }
	double getScore() { return m_score; }

	void Mutate(double rate);

	vector<double> getLastLayer();

	//in seconds
	//call after propigate and mutate
	double GetSpeed();

	void saveNet(std::string name);
	Nnet loadNet(std::string filename);

	int GetInputSize() { return input.size; }
	int GetOutputSize() { return output.size; }

protected:
	//const int MAX_WEIGHT = 9999;
	int version = 2;// increment when save files change MANUALLY

	double trate;

	double m_score;

	int age;

	clock_t startProp, startMut;
	double PassedProp, PassedMut;

	//layers
	layer input;
	layer output;
	vector<layer> hidden;

	int m_Nhidden;

	problem_type pt;

	double Normalize(double input);
	bool AddBiases(vector<double> cur, vector<double> biases, vector<double> &out);

	void MutTable(double &weight);
};