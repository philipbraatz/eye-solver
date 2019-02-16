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


        protected void AddBiases(List<double> cur, List<double> biases, ref List<double> output)
        {
            try
            {
                if (cur.Count == biases.Count)
                    for (int i = 0; i < cur.Count; i++)
                        output.Add(cur[i] + biases[i]);
                else
                    throw new Exception("ERROR: Invalid List sizes");
            }
            catch (Exception exception)
            {
                Console.WriteLine(exception);
                throw;
            }
        }

        public Reader(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs)
        {
            if (num_inputs == 0 || num_hiddens == 0 || num_nodesHidden == 0 || num_outputs == 0) //if an inputLayer is missing
                throw new Exception("Invalid Reader: All network layers have to be greater than 0");

            //reset layers
            inputLayer=new Layer();
            outputLayer = new Layer();
            hiddenLayers = new Chunk();

            //resize weights
            //randomize weightsH
            //randomize biases
            inputLayer.Fill(num_inputs, num_nodesHidden);
            outputLayer.Fill(num_outputs, 0);//no weights come out last layer
            hiddenLayers.Fill(num_hiddens, num_nodesHidden, num_nodesHidden);
            hiddenLayers[num_hiddens - 1].Fill(num_nodesHidden, num_outputs);//last hidden layer
        }
        public Reader(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs,double fillValue)
        {
            if (num_inputs == 0 || num_hiddens == 0 || num_nodesHidden == 0 || num_outputs == 0) //if an inputLayer is missing
                throw new Exception("Invalid Reader: All network layers have to be greater than 0");

            //reset layers
            inputLayer = new Layer();
            outputLayer = new Layer();
            hiddenLayers = new Chunk();

            //resize weights
            //randomize weightsH
            //randomize biases
            inputLayer.Fill(num_inputs, num_nodesHidden,fillValue);
            outputLayer.Fill(num_outputs, num_outputs, fillValue);
            hiddenLayers.Fill(num_hiddens, num_nodesHidden, num_nodesHidden, fillValue);
            hiddenLayers[num_hiddens - 1].Fill(num_nodesHidden, num_outputs, fillValue);//last hidden layer
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
            for (int b = 0; b < inputLayer.Count; b++)
                biases.Add( inputLayer[b].bias);
            AddBiases(inputs, biases,ref output);
            for (int i = 0; i < inputLayer.Count; i++)
                inputLayer[i].value = algs.Normalize(output[i]);

            //First Hidden Layer
            for (int i = 0; i < hiddenLayers.First().Count; i++)
            {
                adder = 0;
                for (int j = 0; j < inputLayer.Count; j++)
                    adder += inputLayer[j].value * inputLayer[j].weights[i];//prevnode * weight
                adder += hiddenLayers.First()[i].bias;

                hiddenLayers[0][i].value = algs.Normalize(adder);
            }

            //Hidden Layers
            for (int i = 1; i < hiddenLayers.Count; i++)
                for (int j = 0; j < hiddenLayers[i].Count; j++)
                {
                    adder = 0;
                    for (int k = 0; k < hiddenLayers[i].Count; k++)
                        adder += hiddenLayers[i - 1][j].value * hiddenLayers[i - 1][j].weights[k];
                    adder += hiddenLayers[i][j].bias;

                    hiddenLayers[i][j].value = algs.Normalize(adder);
                }

            List<double> ret=new List<double>();
            //Output Layer
            for (int i = 0; i < outputLayer.Count; i++)
            {
                adder = 0;
                for (int j = 0; j < hiddenLayers.Last().Count; j++)
                    adder += hiddenLayers.Last()[j].value * hiddenLayers.Last()[j].weights[i];//prevnode*weight
                adder += outputLayer[i].bias;
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