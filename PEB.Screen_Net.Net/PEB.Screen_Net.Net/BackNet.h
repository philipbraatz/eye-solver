#pragma once
#include "Nnet.h"

struct Weight {
	double current;
	double prevDelta;
	double delta;
};

struct Neuron {

	//double value;   //Local input
	double bias;
	double gradient;
	double value; //Output value
	vector<Weight> weight;    //path array 
	int nSize; //number of paths/connections belonging to *this
};

struct Layerl {

	int size;   //number of neurons in cluster/Layerl
	vector<Neuron> neurons;   //Local Neuron array to reference
	vector<Layerl> neighbor;

	void Transfer(int id)  //compute target Neuron's input and assign it to it
	{
		double valueOut = 0;
		vector<Neuron> temp;
		temp = neurons;//temp[i] = n[nid];

		//for each connection, send w*v to paired neuron
		for (int i = 0; i < neurons[id].nSize; i++)
		{
			valueOut = temp[i].weight[i].current * temp[i].value;

			//neuron nid(as referenced by p[]), which is in the other Layerl, receives the value
			neighbor[i].neurons[i].value += valueOut;
		}
	}

	void Initialize(int size)
	{

		size = size;
		neurons.resize(size);

		for (int i = 0; i < size; i++)
		{
			neurons[i].bias = 1.0;
			neurons[i].value = 0.0;
		}
	}

	void FormConnections(Layerl& nl)//with neighboring Layerl
	{
		neighbor.push_back(nl);

		int nCon = neighbor.back().size;
		for (int i = 0; i < size; i++)
		{
			neurons[i].nSize = nCon;
			neurons[i].weight.resize(nCon);

			//neuron 'i' will link its paths to neurons in the other Layerl
			for (int ii = 0; ii < neurons[i].nSize; ii++)
			{
				neurons[i].weight[ii].current = 1.0;
				neurons[i].weight[ii].prevDelta = 0.0;
			}
		}
	}
};

class Brain {
public:

	double eta;
	double alpha;
	Layerl   input, hidden, output;

	vector<double> target;

	void GetInput(vector<double> in)
	{
		for (int i = 0; i < input.size; i++)
			input.neurons[i].value = in[i];
	}

	void GetDesiredOutput(vector<double> t) { target = t; }

	void Initialize(int inputsize, int hiddensize, int outputsize)
	{
		input.Initialize(inputsize);
		hidden.Initialize(hiddensize);
		output.Initialize(outputsize);

		input.FormConnections(hidden);
		hidden.FormConnections(output);
	}

	void BP()
	{
		//Calculate gradients for output
		for (int i = 0; i < output.size; i++)
			output.neurons[i].gradient =
			(target[i] - output.neurons[i].value) *
			(1 - output.neurons[i].value) *
			(1 + output.neurons[i].value);

		vector<Neuron> temp;
		for (int i = 0; i < hidden.size; i++)
		{
			temp[i] = hidden.neurons[i];//temp = &hidden.n[i];
			temp[i].gradient = 0;

			//for each connection...
			for (int ii = 0; ii < hidden.neurons[i].nSize; ii++)
				//temp's gradient gets values in the form w1*g2 + w2*g2 + ... + wn*gn,
				//where w is the weight of p that leads to output.n[i] from temp(hidden), and g
				//is the gradient of that output at p[CurrentConnection].nid
				temp[i].gradient +=
				temp[i].weight[ii].current *
				output.neurons[ii].gradient;
			//Multiply the resultant sums with d/dx S(x)
			temp[i].gradient *= (temp[i].value)*(1 - temp[i].value);


		}
		//Calculate delta
		for (int i = 0; i < input.size; i++)
		{
			temp[i] = input.neurons[i];



			for (int ii = 0; ii < input.neurons[i].nSize; ii++)
			{
				temp[i].weight[ii].delta = eta *
					hidden.neurons[ii].gradient*
					temp[i].value;
				temp[i].weight[ii].current +=
					temp[i].weight[ii].prevDelta* alpha +
					temp[i].weight[ii].delta;
				temp[i].weight[ii].prevDelta = temp[i].weight[ii].delta;
			}
		}

		double pbd=0;
		for (int i = 0; i < hidden.size; i++)
		{
			temp[i] = hidden.neurons[i];
			double bd = eta * temp[i].gradient;

			temp[i].bias += bd + alpha * pbd;
			pbd = bd;

			for (int ii = 0; ii < hidden.neurons[i].nSize; ii++)
			{
				temp[i].weight[ii].delta = eta *
					output.neurons[ii].gradient*
					temp[i].value;
				temp[i].weight[ii].current +=
					temp[i].weight[ii].prevDelta*alpha +
					temp[i].weight[ii].delta;
				temp[i].weight[ii].prevDelta = temp[i].weight[ii].delta;
			}
		}

		for (int i = 0; i < output.size; i++)
		{
			temp[i] = output.neurons[i];
			temp[i].bias = eta * temp[i].gradient;
		}

		Zero(hidden);
		Zero(output);
	}

	void Process()
	{
		for (int i = 0; i < input.size; i++)
			input.Transfer(i);//transfer each neuron data in input to hidden
		for (int i = 0; i < hidden.size; i++)
		{
			hidden.neurons[i].value = Sigmoid(hidden.neurons[i].value + hidden.neurons[i].bias);
			hidden.Transfer(i);
		}

		for (int i = 0; i < output.size; i++)//{
			output.neurons[i].value = HyperTan(output.neurons[i].value + output.neurons[i].bias);
		//std::cout << "Output " << i << ": " << output.neurons[i].value << std::endl;//debug
	//}
	}

	void Zero(Layerl &l) { for (int i = 0; i < l.size; i++) l.neurons[i].value = 0.0; }
	void Randomize(Layerl &l)
	{
		for (int i = 0; i < l.size; i++)
			for (int ii = 0; ii < l.neurons[i].nSize; ii++)
				l.neurons[i].weight[ii].current = rand() % 100 / 10;
	}
	Brain() { eta = 0.9; alpha = 0.4; }

	double Sigmoid(double x)
	{
		if (x < -45.0) return 0.0;
		else if (x > 45.0) return 1.0;
		else return (1.0 / (1.0 + pow(e, -x)));
	}
	double HyperTan(double x)
	{
		if (x < -10.0) return -1.0;
		else if (x > 10.0) return 1.0;
		else return tanh(x);
	}
};

void RunBrain()
{
	Brain b;

	vector<double> data = { 1.0,2.0, 3.0 };
	vector<double> ddata = { -0.25,0.14 };

	b.Initialize(3, 4, 2);

	b.GetDesiredOutput(ddata);
	b.GetInput(data);

	b.Process();
	b.BP();
}


