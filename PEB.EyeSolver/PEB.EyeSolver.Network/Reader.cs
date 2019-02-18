using PEB.EyeSolver.Network;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace PEB.EyeSolver.Network
{
    public class Reader
    {
        protected Random rnd = new Random();

        //const int MAX_WEIGHT = 9999;

        private Stopwatch timer =new Stopwatch();
        protected int passedProp = 0;

        //layers
        protected Layer inputLayer;
        protected Layer outputLayer;
        protected Chunk hiddenLayers;


        protected void AddBiases(ref List<double> layer,double bias)
        {
            List<double> output =new List<double>();
            for (int i = 0; i < layer.Count; i++)
                output.Add(layer[i] + bias);
            layer = output;
        }

        private void Init(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs, double value=-123.456 )
        {
            if (num_inputs == 0 || num_hiddens == 0 || num_nodesHidden == 0 || num_outputs == 0) //if an inputLayer is missing
                throw new Exception("Invalid Reader: All network layers have to be greater than 0");

            //reset layers
            inputLayer = new Layer();
            outputLayer = new Layer();
            hiddenLayers = new Chunk();

            //set layers
            inputLayer.Fill(num_inputs, num_nodesHidden, value);
            outputLayer.Fill(num_outputs,0, value);
            hiddenLayers.Fill(num_hiddens-1, num_nodesHidden, num_nodesHidden, value);
            hiddenLayers.Add(new Layer().Fill(num_nodesHidden, num_outputs, value));//last hidden layer

            double fakevalue = -33;
            fakevalue += 22;
        }

        public Reader(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs)
        {
            Init(num_inputs, num_hiddens, num_nodesHidden, num_outputs);
        }
        public Reader(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs,double fillValue)
        {
            Init( num_inputs,  num_hiddens,  num_nodesHidden,  num_outputs, fillValue);
        }

        public int GetLayerSize(LayerType l)
        {
            switch (l)
            {
                case LayerType.INPUT:
                    return inputLayer.Count;
                case LayerType.HIDDEN_COUNT:
                    return hiddenLayers.Count;
                case LayerType.OUTPUT:
                    return outputLayer.Count;
                case LayerType.HIDDEN_NODES_COUNT:
                    return hiddenLayers.First().Count;
                default:
                    return -1;
            }
        }

        //set inputLayer and get outputLayer
        public List<double> Propigate(List<double> inputs)
        {
            //TIMER first
            timer.Restart();//Start timer
            //END

            //error handling second
            if (inputLayer.Count != inputs.Count)
                throw new Exception("Invalid inputLayer size");

            double adder = 0;//value of node

            /* ORDER
                prev node * weight
                bias
                normalize
                set node
            */

            List<double> biases = new List<double>(),
                output = new List<double>();

            //Input layer
            AddBiases(ref inputs, inputLayer.bias);
            for (int i = 0; i < inputLayer.Count; i++)
                inputLayer[i].value = algs.Normalize( inputs[i]);

            //First Hidden Layer
            for (int i = 0; i < hiddenLayers.First().Count; i++)//output nodes
            {
                adder = 0;
                for (int j = 0; j < inputLayer.Count; j++)//input nodes
                    adder += inputLayer[j].value * inputLayer[j].weights[i];//prevnode * weight
                adder += hiddenLayers.First().bias;

                hiddenLayers[0][i].value = algs.Normalize(adder);
            }

            //Hidden Layers
            for (int i = 0; i < hiddenLayers.Count - 1; i++)//Layer
                for (int j = 0; j < hiddenLayers[i].Count; j++)//output nodes
                {
                    adder = 0;
                    for (int k = 0; k < hiddenLayers[i].Count; k++)//input nodes
                        adder += hiddenLayers[i][k].value * hiddenLayers[i][k].weights[j];//prevnode * weight
                    adder += hiddenLayers[i].bias;

                    hiddenLayers[i][j].value = algs.Normalize(adder);
                }

            //Output Layer
            List<double> ret=new List<double>();
            for (int i = 0; i < outputLayer.Count; i++)
            {
                adder = 0;
                for (int j = 0; j < hiddenLayers.Last().Count; j++)
                    adder += hiddenLayers.Last()[j].value * hiddenLayers.Last()[j].weights[i];//prevnode*weight
                adder += outputLayer.bias;
                outputLayer[i].value = algs.Normalize(adder);// Normalize(adder);//normalize
                ret.Add( outputLayer[i].value);
            }
            timer.Stop();
            passedProp += (int)timer.ElapsedMilliseconds;
            return ret;
        }
        public List<double> Initial_Propigation(List<double> inputs) { return Propigate(inputs); }//for inheritience
        public List<double> Final_Propigation(List<double> inputs) { return Propigate(inputs); }//for inheritience
       

        public List<double> getLastLayer()
        {
            List<double> output =new List<double>();
            for (int i = 0; i < outputLayer.Count; i++)
            {
                output.Add(outputLayer[i].value);
            }
            return output;
        }

        //in seconds
        //call after propigate and mutate
        public double GetTime()
        {
            double total = passedProp;
            passedProp = 0;  //prevents double counting	//
            return total;
        }
    }
}