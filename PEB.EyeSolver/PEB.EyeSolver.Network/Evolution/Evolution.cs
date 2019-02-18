using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PEB.EyeSolver.Network.Evolution
{

    public class Evolution:
        List<Mutator>
    {
        public int setME = 0;
        private List<List<double>> propOut = new List<List<double>>();

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

        public List<List<double>> Propigate(List<double> inputs, List<double> goals)
        {
            if (inputs.Count != this.First().GetLayerSize(LayerType.INPUT) ||
                goals.Count != this.First().GetLayerSize(LayerType.OUTPUT))

            foreach (var network in this)
                propOut.Add( network.Propigate(inputs, goals));
            return propOut;
        }

        public new Evolution Sort()
        {
            return new Evolution( 
                this.OrderBy(o => o.getError()).ToList(),
                this.First().mut_rate);
        }

        public List<double> getError()
        {
            List<double> error = new List<double>();
            foreach (var network in this)
                error.Add( network.getError());
            return error;
        }

        public void Mutate()
        {
            foreach (var network in this)
                network.Mutate();
        }
    }
}
