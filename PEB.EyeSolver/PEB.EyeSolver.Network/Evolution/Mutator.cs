using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace PEB.EyeSolver.Network.Evolution
{
    public static class algs
    {
        public static readonly double e = 2.71828182845904523536;

        public static double Normalize(double input) => input = 1 / (1 + Math.Pow(e, -input));
    }

    public class Mutator:
        Reader
    {
        private int passedMut =0;
        private double mut_rate;
 
        private Stopwatch timer =new Stopwatch();

        public Mutator(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs, double mutate_rate) :
            base(num_inputs, num_hiddens, num_nodesHidden, num_outputs)
        {
            mut_rate = mutate_rate;
        }

        public void Mutate()
        {
            //TIMER first
            timer.Restart(); //Start timer

            for (int i = 0; i < inputLayer.Count; i++)
            {//input
                for (int j = 0; j < hiddenLayers.First().Count; j++)
                {
                    if (mut_rate > rnd.NextDouble()) //if rand is less that its mutate rate
                    {
                        var temp = inputLayer[i].weights[j];
                        MutTable(ref temp); //mutate
                        inputLayer[i].weights[j] = temp;
                    }
                }
            }
            for (int i = 1; i < hiddenLayers.Count; i++)
            {//hidden
                for (int j = 0; j < hiddenLayers[i].Count; j++)
                {
                    for (int k = 0; k < hiddenLayers[i].Count; k++)
                    {

                        if (mut_rate > rnd.NextDouble()) //if rand is less that its mutate rate
                        {
                            var temp = hiddenLayers[i - 1][j].weights[k];
                            MutTable(ref temp); //mutate
                            hiddenLayers[i - 1][j].weights[k] = temp;
                        }
                    }
                }
            }
            for (int i = 0; i < hiddenLayers.Last().Count; i++)
            {//last hidden
                for (int j = 0; j < outputLayer.Count; j++)
                {
                    if (mut_rate > rnd.NextDouble()) //if rand is less that its mutate rate
                    {
                        var temp = hiddenLayers.Last()[i].weights[j];
                        MutTable(ref temp); //mutate
                        hiddenLayers.Last()[i].weights[j] = temp;
                    }

                    //std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b";
                    //std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b";
                }
            }

            //biases
            for (int i = 0; i < inputLayer.Count; i++)
            {
                if (mut_rate > rnd.NextDouble())
                    MutTable(ref inputLayer[i].bias);
            }
            for (int i = 0; i < hiddenLayers.Count; i++)
            {
                for (int j = 0; j < hiddenLayers[i].Count; j++)
                {
                    if (mut_rate > rnd.NextDouble())
                    {
                        var temp = hiddenLayers[i][j].bias;
                        MutTable(ref temp);
                        hiddenLayers[i][j].bias = temp;
                    }
                }
            }
            for (int i = 0; i < outputLayer.Count; i++)
            {
                if (mut_rate > rnd.NextDouble())
                {
                    var temp = outputLayer[i].bias;
                    MutTable(ref temp);
                    outputLayer[i].bias = temp;
                }
            }
            timer.Stop();
            passedMut += (int)timer.ElapsedMilliseconds;// / CLOCKS_PER_SEC;
        }

        public double MutTable(ref double weight)
        {
            const int MAX_WEIGHT = 4;

            int choice = (int)(rnd.Next(0, 5));
            double r = rnd.NextDouble()+0.5;

            //limit min/max
            if (weight > MAX_WEIGHT)
                weight = MAX_WEIGHT;
            else if (weight < -MAX_WEIGHT)
                weight = -MAX_WEIGHT;

            switch (choice)
            {
                case 0://weight = 25% to 225%
                    return weight *= r*r;
                case 1://invert weight
                    return weight *= -1;
                case 2://massively increase/ decrease
                    return weight = Math.Pow(weight, r);
                case 3://increase slightly
                    return weight *=1-r/1000;
                case 4://decrease slightly
                    return weight *=1+r/1000;
                default:
                    throw new Exception("Unreachable Code reached");
            }
        }

        //inheritance
        public new double GetTime()
        {
            double total = passedMut + passedProp;
            passedMut = 0;  //prevents double counting	//
            passedProp = 0; //							//
            return total;
        }
    }
}
