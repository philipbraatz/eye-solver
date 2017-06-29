#pragma once
#include <vector>

#include <string>//Save/Load file
#include <fstream>
#include <iostream>//DEBUG

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

	void setScore(float score) { m_score = score; }
	float getScore() { return m_score; }

	void saveNet(std::string name)
	{
		name.append(".bin");
		std::ofstream out(name, std::ios_base::binary);
		if (out.good())
		{
			//save sizes
			std::cout << "Writing Sizes of file" << std::endl;			//ORDER
			out.write((char *)&input.size, sizeof(unsigned int));			//input layer size
			out.write((char *)&output.size, sizeof(unsigned int));			//output layer size
			out.write((char *)&hidden.front().size, sizeof(unsigned int));	//hidden layers size
			out.write((char *)&m_Nhidden, sizeof(unsigned int));				//number of hidden layers

			std::cout << "Writing  biases" << std::fixed << std::endl;	//biases
			for (size_t i = 0; i < input.size; i++)
				out.write((char *)&input.neurons[i].bias, sizeof(float));
			for (size_t i = 0; i < output.size; i++)
				out.write((char *)&output.neurons[i].bias, sizeof(float));
			for (size_t i = 0; i < m_Nhidden; i++)
				for (size_t j = 0; j < hidden[i].size; j++)
					out.write((char *)&output.neurons[i].bias, sizeof(float));

			std::cout << "Writing  weights" << std::fixed << std::endl;	//weights
			for (size_t i = 0; i < input.size; i++)
				for (size_t j = 0; j < hidden.front().size; j++)
					out.write((char *)&input.neurons[i].weights[j], sizeof(float));
			for (size_t i = 0; i < output.size; i++)
				for (size_t j = 0; j < hidden.back().size;j++)
					out.write((char *)&output.neurons[i].weights[j], sizeof(float));
			for (size_t i = 0; i < m_Nhidden-1; i++)
				for (size_t j = 0; j < hidden[i].size; ++j)
					for (size_t k = 0; k < hidden[i+1].size; k++)
						hidden[i].neurons[j].weights[k];
			for (size_t i = 0; i < hidden.back().size; i++)//Last Hidden Layer
				for (size_t j = 0; j < output.size; j++)
					hidden.back().neurons[i].weights[j];

			out.close();
		}
	}
	void loadNet(std::string filename)
	{
		unsigned int sizeInput;
		unsigned int sizeOutput;
		unsigned int sizeHidden;
		unsigned int numHidden;

		std::ifstream in(filename, std::ios_base::binary);
		if (in.good())
		{
			//Get Nnet sizes
			std::cout << "Reading sizes" << std::endl;
			in.read((char *)&sizeInput, sizeof(unsigned int));
			in.read((char *)&sizeOutput, sizeof(unsigned int));
			in.read((char *)&sizeHidden, sizeof(unsigned int));
			in.read((char *)&numHidden, sizeof(unsigned int));

			//Resize Nnet
			Setup(sizeInput, numHidden, sizeHidden, sizeOutput, true);

			//Load Biases
			std::cout << "Reading  biases: " << std::fixed << std::endl;
			for (size_t i = 0; i < input.size; i++)
				in.read((char *)&input.neurons[i].bias, sizeof(float));
			for (size_t i = 0; i < output.size; i++)
				in.read((char *)&output.neurons[i].bias, sizeof(float));
			for (size_t i = 0; i < m_Nhidden; i++)
				for (size_t j = 0; j < hidden[i].size; j++)
					in.read((char *)&output.neurons[i].bias, sizeof(float));

			//Load Weights
			std::cout << "Reading  weights: " << std::fixed << std::endl;
			for (size_t i = 0; i < input.size; i++)
				for (size_t j = 0; j < hidden.front().size; j++)
					in.read((char *)&input.neurons[i].weights[j], sizeof(float));
			for (size_t i = 0; i < output.size; i++)
				for (size_t j = 0; j < hidden.back().size; j++)
					in.read((char *)&output.neurons[i].weights[j], sizeof(float));
			for (size_t i = 0; i < m_Nhidden; i++)
				for (size_t j = 0; j < hidden[i].size; ++j)
					for (size_t k = 0; k < hidden[i + 1].size; k++)
						hidden[i].neurons[j].weights[k];
		}
	}

	void Mutate(double rate);

	vector<float> Nnet::getLastLayer();

private:
	//const int MAX_WEIGHT = 9999;

	float m_score;

	//layers
	layer input;
	layer output;
	vector<layer> hidden;

	unsigned int m_Nhidden;

	void Normalize(float &input);
	void AddBiases(vector<float> cur, vector<float> biases, vector<float> &out);

	void MutTable(float &weight);


};