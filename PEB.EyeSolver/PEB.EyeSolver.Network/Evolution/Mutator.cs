using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
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
        public double mut_rate;
        private double error=0;
 
        private Stopwatch timer =new Stopwatch();

        public List<double> Propigate(List<double> inputs,List<double> goals)
        {
            error = 0;
            List<double> output = this.Propigate(inputs);//propigation
            for (int i = 0; i < goals.Count; i++)
                error += Math.Pow(goals[i] - output[i], 2);//calc error
            if (error == 0)
            {
                throw  new Exception("Perfected learning");
            }
            return output;
        }

        public Mutator(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs, double mutate_rate) :
            base(num_inputs, num_hiddens, num_nodesHidden, num_outputs)
        {
            mut_rate =mutate_rate;
        }
        public Mutator(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs,double fill, double mutate_rate) :
            base(num_inputs, num_hiddens, num_nodesHidden, num_outputs,fill)
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
            if (mut_rate > rnd.NextDouble())//INPUT
                MutTable(ref inputLayer.bias);
            for (int i = 0; i < hiddenLayers.Count; i++)//HIDDEN
                if (mut_rate > rnd.NextDouble())
                    MutTable(ref hiddenLayers[i].bias);
            if (mut_rate > rnd.NextDouble())//OUTPUT
                MutTable(ref outputLayer.bias);
            timer.Stop();
            passedMut += (int)timer.ElapsedMilliseconds;// / CLOCKS_PER_SEC;
        }

        private double MutTable(ref double weight)
        {
            const int MAX_WEIGHT = 8;

            int choice = (int)(rnd.Next(0, 6));
            double r = rnd.NextDouble()+0.5;

            //limit min/max
            if (weight > MAX_WEIGHT)
                weight = MAX_WEIGHT;
            else if (weight < 0)
                weight = rnd.NextDouble() / 1000;

            switch (choice)
            {
                case 0://weight = 25% to 225%
                    return weight *= r*r;
                case 1://low weight
                    return weight = r / 1000;
                case 2://high weight
                    return weight = 1.5-r / 1000;
                case 3://massively increase/ decrease
                    return weight = Math.Pow(weight, r);
                case 4://increase slightly
                    return weight *=1-r/1000;
                case 5://decrease slightly
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

        public double getError()
        {
            return error;
        }
    }
}
