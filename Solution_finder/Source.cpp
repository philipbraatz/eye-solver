//#include "Nnet.h"
//#include <vector>
//
//void DoThing()
//{
//	Nnet test(2, 2, 2, 2);
//	std::vector<float> input = { 1,0 };
//	test.Propigate(input);
//	test.train();
//}
//
////Main training function. Run this function in a loop as many times needed per pattern
//float Nnet::train(std::vector<float> desiredoutput, float alpha, float momentum)
//{
//	//function train, teaches the network to recognize a pattern given a desired output
//	float errorg = 0; //general quadratic error
//	float errorc; //local error;
//	float sum = 0, csum = 0;
//	float delta, udelta;
//	float output;
//	//first we begin by propagating the input
//	//Nnet::propagate(input);
//	int i, j, k;
//	//the backpropagation algorithm starts from the output layer propagating the error  from the output
//	//layer to the input layer
//	for (i = 0; i<m_Noutputs; i++)
//	{
//		//calculate the error value for the output layer
//		output = outputNodes[i]; //copy this value to facilitate calculations
//												   //from the algorithm we can take the error value as
//		errorc = (desiredoutput[i] - output) * output * (1 - output);
//		//and the general error as the sum of delta values. Where delta is the squared difference
//		//of the desired value with the output value
//		//quadratic error
//		errorg += (desiredoutput[i] - output) * (desiredoutput[i] - output);
//
//		//now we proceed to update the weights of the neuron
//		for (j = 0; j<m_SizeHidden; j++)
//		{
//			//get the current delta value
//			delta = outputNodes[i].deltavalues[j];
//			//update the delta value
//			udelta = alpha * errorc * m_outputlayer.layerinput[j] + delta * momentum;
//			//update the weight values
//			outputWeights[i][j] += udelta;
//			outputWeights[i].deltavalues[j] = udelta;
//
//			//we need this to propagate to the next layer
//			sum += m_outputlayer.neurons[i]->weights[j] * errorc;
//		}
//
//		//calculate the weight gain
//		m_outputlayer.neurons[i]->wgain += alpha * errorc * m_outputlayer.neurons[i]->gain;
//
//	}
//
//	for (i = (m_hiddenlayercount - 1); i >= 0; i--)
//	{
//		for (j = 0; j<m_hiddenlayers[i]->neuroncount; j++)
//		{
//			output = m_hiddenlayers[i]->neurons[j]->output;
//			//calculate the error for this layer
//			errorc = output * (1 - output) * sum;
//			//update neuron weights
//			for (k = 0; k<m_hiddenlayers[i]->inputcount; k++)
//			{
//				delta = m_hiddenlayers[i]->neurons[j]->deltavalues[k];
//				udelta = alpha * errorc * m_hiddenlayers[i]->layerinput[k] + delta * momentum;
//				m_hiddenlayers[i]->neurons[j]->weights[k] += udelta;
//				m_hiddenlayers[i]->neurons[j]->deltavalues[k] = udelta;
//				csum += m_hiddenlayers[i]->neurons[j]->weights[k] * errorc;//needed for next layer
//
//			}
//
//			m_hiddenlayers[i]->neurons[j]->wgain += alpha * errorc * m_hiddenlayers[i]->neurons[j]->gain;
//
//		}
//		sum = csum;
//		csum = 0;
//	}
//
//	//and finally process the input layer
//	for (i = 0; i<m_inputlayer.neuroncount; i++)
//	{
//		output = m_inputlayer.neurons[i]->output;
//		errorc = output * (1 - output) * sum;
//
//		for (j = 0; j<m_inputlayer.inputcount; j++)
//		{
//			delta = m_inputlayer.neurons[i]->deltavalues[j];
//			udelta = alpha * errorc * m_inputlayer.layerinput[j] + delta * momentum;
//			//update weights
//			m_inputlayer.neurons[i]->weights[j] += udelta;
//			m_inputlayer.neurons[i]->deltavalues[j] = udelta;
//		}
//		//and update the gain weight
//		m_inputlayer.neurons[i]->wgain += alpha * errorc * m_inputlayer.neurons[i]->gain;
//	}
//
//	//return the general error divided by 2
//	return errorg / 2;
//
//}