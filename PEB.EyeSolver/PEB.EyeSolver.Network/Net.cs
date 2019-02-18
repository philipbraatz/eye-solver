using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace PEB.EyeSolver.Network
{
    public enum LayerType
    {
        INPUT, HIDDEN_COUNT, OUTPUT, HIDDEN_NODES_COUNT
    };
    public enum problem_type
    {
        TEXT,
        IMAGE,
        VIDEO,
        OTHER
    };

    //FOR MENU move elsewhere if able
    public struct NetFrame
    {
        public int input, output, size, hiddens, hsize;
        public double rate;
        /*string textAnswer;
        Mat imgAnswer;*/
        public problem_type type;
    };

    /// <summary>
    ///holds current value and weights for next layer
    /// </summary>
    public class Neuron
    {
        public double value=0;
        public List<double> weights=new List<double>();

        //double> deltavalues;
        private Random r = new Random();
        //initializes randomly
        public void Fill(int weightSize)
        {
            for (int i = 0; i < weightSize; i++)
                weights.Add(r.NextDouble());
        }

        //recommend for killing with 0 or 1
        public void Fill(int weightSize, double value=-123.456)
        {
            Neuron n = new Neuron();
            for (int i = 0; i < weightSize; i++)
                if(value != -123.456)
                    weights.Add(value);
                else
                    weights.Add(r.NextDouble());
        }

    };

    public class Layer : List<Neuron>
    {
        private Random r = new Random();
        public double bias = 0;

        public Layer Fill(int layerSize, int weightSize, double value= -123.456)
        {
            for (int i = 0; i < layerSize; i++)
            {
                Neuron n =new Neuron();
                if (value != -123.456)
                    n.Fill(weightSize, value);
                else
                    n.Fill(weightSize);
                this.Add(n);
            }
            if (value != -123.456)
                bias = value;
            else
                bias = r.NextDouble();
            return this;
        }
    };

    public class Chunk : List<Layer>
    {
        public void Fill(int chunkSize,int layerSize, int weightSize, double value = -123.456)
        {
            for (int i = 0; i < chunkSize; i++)
            {
                Layer l =new Layer();
                if(value != -123.456)
                    l.Fill(layerSize, weightSize, value);
                else
                    l.Fill(layerSize, weightSize);
                this.Add(l);
            }
                
        }
    };

    public static class algs
    {
        public static readonly double e = 2.71828182845904523536;

        public static double Normalize(double input) => input = 1 / (1 + Math.Pow(e, -input));
        public static void Normalize(ref List<double> input)
        {
            for (int i = 0; i < input.Count; i++)
                input[i] = 1 / (1 + Math.Pow(e, -input[i]));
        } 
    }
}
