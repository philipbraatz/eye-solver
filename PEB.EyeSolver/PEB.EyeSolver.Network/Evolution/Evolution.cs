using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PEB.EyeSolver.Network.Evolution
{

    public class Evolution:
        List<Mutator>
    {
        private int generation = 0;
        private List<List<double>> propOut = new List<List<double>>();
        private Random r =new Random();

        private Stopwatch timer =new Stopwatch();

        public Evolution(int population,int input,int hiddenLayers,int hiddenNodes,int output, double mutRate)
        {
            for (int i = 0; i < population; i++)
                this.Add(new Mutator( input, hiddenLayers, hiddenNodes, output, mutRate));
            
        }
        public Evolution(int population, int input, int hiddenLayers, int hiddenNodes, int output,double fill, double mutRate)
        {
            for (int i = 0; i < population; i++)
                this.Add(new Mutator(input, hiddenLayers, hiddenNodes, output,fill, mutRate));
        }
        public Evolution(List<Mutator> newEvo,double mutRate)
        {
            this.AddRange(newEvo);
            foreach (var network in this)
                network.mut_rate =mutRate;
        }

        public void doEpoc(List<double> inputs, List<double> goals,double bottomPercent)
        {
            timer.Restart();
            this.Propigate(inputs, goals);
            Sort();
            Mutate(bottomPercent);
            generation++;
            timer.Stop();
        }

        public List<List<double>> Propigate(List<double> inputs, List<double> goals)
        {
            if (inputs.Count != this.First().GetLayerSize(LayerType.INPUT) ||
                goals.Count != this.First().GetLayerSize(LayerType.OUTPUT))
                throw new Exception("parameters do not make network size");
            foreach (var network in this)
                propOut.Add( network.Propigate(inputs, goals));
            return propOut;
        }

        public new void Sort()
        {
            var evo = this.OrderBy(o => o.getError()).ToList();
            for (int i = 0; i < this.Count; i++)
                this[i] = evo[i];
        }

        public List<double> getError()
        {
            List<double> error = new List<double>();
            foreach (var network in this)
                error.Add( network.getError());
            return error;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="bottomPercent">50% should be best</param>
        public void Mutate(double bottomPercent)
        {
            int bottom = (int)(this.Count * bottomPercent);//remove this many networks
            int top = (int) (this.Count * .01);//gets the top 1%

            this.RemoveRange(0,bottom);
            for (int i = 0; i < bottom; i++)
            {
                //using i squared to increase chances of using better networks
                int clone = r.Next(
                    (int)Math.Pow(i, 2) < this.Count ? 
                    (int)Math.Pow(i, 2): this.Count-top,
                    this.Count
                    );
                this.Add(this[clone]);//fill with new clones
                this.Last().Mutate();//mutate them
            }
            
        }

        public long getTime()
        {
            return timer.ElapsedMilliseconds;
        }
    }
}
