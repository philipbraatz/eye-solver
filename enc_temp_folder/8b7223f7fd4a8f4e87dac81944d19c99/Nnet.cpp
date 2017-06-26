#include "Nnet.h"
#include "utility.h"

#include <vector>
#include <algorithm>
using std::vector;

	//initialize

	Nnet::Nnet(float Ninputs, float Nhiddens, int SizeHidden, float Noutputs) { Setup(Ninputs, Nhiddens,SizeHidden, Noutputs);}
	Nnet::Nnet(std::string filename) {
		loadNet(filename);
		Setup(0,0,0,0,true);
	}
	//Nnet::Nnet(float Ninputs, float Nhiddens, int SizeHidden, float Noutputs, vector<float> goal) {
	//	m_goal = goal;
	//	Setup(Ninputs, Nhiddens, SizeHidden, Noutputs);
	//}

	void Nnet::Setup(float Ninputs, float Nhiddens, int SizeHidden, float Noutputs, bool loaded)
	{
		//set sizes
		input.size = Ninputs;
		output.size = Noutputs;
		m_Nhidden = SizeHidden;

		//resize layers
		input.neurons.resize(input.size);
		output.neurons.resize(output.size);
		hidden.resize(m_Nhidden);
		for (size_t i = 0; i < m_Nhidden; i++)
		{
			hidden[i].size = SizeHidden;
			hidden[i].neurons.resize(hidden[i].size);
		}
		//resize weights
		//randomize weights
		//randomize biases
		for (size_t i = 0; i < input.size; i++)
		{//INPUT
			input.neurons[i].weights.resize(hidden.front().size);
			if (!loaded)
			{
				for (size_t j = 0; j < hidden.front().size; j++)
				{
					input.neurons[i].weights[j] = RandNum();
				}
				input.neurons[i].bias = RandNum();
			}
		}
		for (size_t i = 0; i < output.size; i++)
		{//OUTPUT
			output.neurons[i].weights.resize(hidden.back().size);
			if (!loaded)
			{
				for (size_t j = 0; j < hidden.back().size; j++)
				{
					output.neurons[i].weights[j] = RandNum();
				}
				output.neurons[i].bias = RandNum();
			}
		}
		for (size_t i = 0; i < m_Nhidden-1; i++)
		{//HIDDEN
			for (size_t j = 0; j < hidden[i].size; j++)
			{
				hidden[i].neurons[j].weights.resize(hidden[i].size);
				if (!loaded)
				{
					for (size_t k = 0; k < hidden[i].size; k++)
					{
						hidden[i].neurons[j].weights[k] = RandNum();
					}
					hidden[i].neurons[j].bias = RandNum();
				}
			}
		}
		for (size_t i = 0; i < hidden.back().size; i++)
		{//LAST HIDDEN
			hidden.back().neurons[i].weights.resize(output.size);
			if (!loaded)
			{
				for (size_t j = 0; j < hidden[i].size; j++)
				{
					hidden.back().neurons[i].weights[j] = RandNum();
				}
				hidden.back().neurons[i].bias = RandNum();
			}
		}
	}

	unsigned int Nnet::GetLayerSize(type l)
	{
		switch (l)
		{
		case Ninputs:
			return input.size;
			break;
		case Nhiddens:
			return m_Nhidden;
			break;
		case Noutputs:
			return output.size;
			break;
		case SizeHidden:
			return hidden.front().size;
			break;
		default:
			break;
		}
	}

	//set input and get output
	std::vector<float> Nnet::Propigate(vector<float> inputs)
	{
		std::vector<float> biases;
		std::vector<float> out;
		
		/* ORDER
			prev node * weight
			bias
			normalize
			set node
		*/

		//Input layer
		biases.resize(input.size);
		bool test = input.size == input.neurons.size();
		for (size_t b = 0; b < input.size; b++)
		{
			biases[b] = input.neurons[b].bias; 
		}
		AddBiases(inputs, biases, out);
		for (size_t i = 0; i < input.size; i++)
		{
			Normalize(out[i]);
			input.neurons[i].value = out[i];
		}

		//First Hidden Layer
		for (size_t i = 0; i < hidden.front().size; i++)
		{
			float adder =0 ;
			for (size_t j = 0; j < input.size; j++)
				 adder += input.neurons[j].value * input.neurons[j].weights[i];//node*weight
			adder += hidden.front().neurons[i].bias;
			Normalize(adder);
			hidden.front().neurons[i].value = adder;
		}

		//Hidden Layers
		for (size_t i = 1; i < m_Nhidden; i++) {
			for (size_t j = 0; j < hidden[i].size; j++) {
				float adder=0;
				for (size_t k = 0; k < hidden[i].size; k++)
					 adder += hidden[i - 1].neurons[j].value * hidden[i - 1].neurons[j].weights[k];
				adder += hidden[i].neurons[j].bias;
				Normalize(adder);
				hidden[i].neurons[j].value = adder;
			}
		}

		//Output Layer
		out.resize(output.size);
		for (size_t i = 0; i < output.size; i++)
		{
			float adder=0;
			for (size_t j = 0; j < m_Nhidden; j++)
				adder += hidden.back().neurons[j].value * output.neurons[i].weights[j];//node*weight
			adder += output.neurons[i].bias;
			Normalize(adder);//normalize
			output.neurons[i].value = adder;
			out[i] = adder;
		}
		return out;
	}

	void Nnet::Normalize(float &input)
	{
		input = 1 / (1 + pow(e, -input));
	}
	void Nnet::AddBiases(vector<float> cur,vector<float> biases, vector<float> &out)
	{
		if (cur.size() == biases.size())
		{
			for (size_t i = 0; i < cur.size(); i++)
			{
				out.push_back(cur[i]+biases[i]);
			}
		}
		else
		{
			//TODO error invalid size
		}

		//std::transform(cur.begin(), cur.end(), biases.begin(), out.begin(), std::plus<float>());
	}