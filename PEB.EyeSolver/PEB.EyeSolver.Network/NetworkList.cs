using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PEB.EyeSolver.Network
{
    class NetworkList<T>:
        List<T> where T :
        Reader
    {
        

        NetworkList(int population,int input,int hiddenLayers,int hiddenNodes,int output)
        {
            for (int i = 0; i < population; i++)
            {
                //this.Add();
            }
        }
    }
}
