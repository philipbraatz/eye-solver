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

    public class Neuron
    {
        public double value=0;
        public List<double> weights=new List<double>();
        public double bias=0;

        //double> deltavalues;

        //initializes randomly
        public void Fill(int weightSize)
        {
            Random r = new Random();
            for (int i = 0; i < weightSize; i++)
                weights.Add(r.NextDouble());
            value = 0;
            bias = r.NextDouble();
        }

        //recommend for killing with 0 or 1
        public void Fill(int weightSize, double value)
        {
            Neuron n = new Neuron();
            for (int i = 0; i < weightSize; i++)
                weights.Add(value);
            value = 0;
            bias = value;
        }

    };

    public class Layer : List<Neuron>
    {
        public void Fill(int layerSize, int weightSize)
        {
            for (int i = 0; i < layerSize; i++)
            {
                Neuron n = new Neuron();
                n.Fill(weightSize);
                this.Add(n);
            }
                
        }

        public void Fill(int layerSize, int weightSize, double value)
        {
            for (int i = 0; i < layerSize; i++)
            {
                Neuron n =new Neuron();
                n.Fill(weightSize, value);
                this.Add(n);
            }
                
        }
    };

    public class Chunk : List<Layer>
    {
        public void Fill(int chunkSize,int layerSize, int weightSize)
        {
            for (int i = 0; i < chunkSize; i++)
            {
                Layer l =new Layer();
                l.Fill(layerSize, weightSize);
                this.Add(l);
            }
                
        }

        public void Fill(int chunkSize, int layerSize, int weightSize, double value)
        {
            for (int i = 0; i < chunkSize; i++)
            {
                Layer l = new Layer();
                l.Fill(layerSize, weightSize,value);
                this.Add(l);
            }
        }
    };

    public static class algs
    {
        public static readonly double e = 2.71828182845904523536;

        public static double Normalize(double input) => input = 1 / (1 + Math.Pow(e, -input));
    }
}
