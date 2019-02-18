using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using PEB.EyeSolver.Network;
using PEB.Network;

namespace PEB.Network.Test
{
    [TestClass]
    public class UTReader
    {
        [TestMethod]
        public void Init()
        {
            Reader net =new Reader(2,3,4,5);
            if(net ==null)
                throw new Exception("Reader is empty");
            Assert.AreEqual(2, net.GetLayerSize(LayerType.INPUT));
            Assert.AreEqual(3, net.GetLayerSize(LayerType.HIDDEN_COUNT));
            Assert.AreEqual(4, net.GetLayerSize(LayerType.HIDDEN_NODES_COUNT));
            Assert.AreEqual(5, net.GetLayerSize(LayerType.OUTPUT));
        }
        [TestMethod]
        public void Propigate()
        {
            double fill = .5;

            double expected = 0.62245;

            Reader net = new Reader(2, 3, 4, 2,fill);
            List<Double> inputs=new List<double>();
            inputs.Add(0.5);
            inputs.Add(0.5);
            List<double> outputs= net.Propigate(inputs);
            foreach (var i in outputs)
                Assert.AreEqual(expected,i,0.00001);
            
        }
    }
}
