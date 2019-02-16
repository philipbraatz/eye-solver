using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PEB.EyeSolver.Network.Evolution
{
    public class Evolution:
        List<Mutator>
    {
        public Evolution(int population,int input,int hiddenLayers,int hiddenNodes,int output, double mutRate)
        {
            this.Add(new Mutator( input, hiddenLayers, hiddenNodes, output, mutRate));
        }

        public void Propigate(List<double> inputs)
        {
            foreach (var network in this)
            {
                network.Propigate(inputs);
            }
        }
    }
}
