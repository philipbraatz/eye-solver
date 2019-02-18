using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using PEB.EyeSolver.Network;
using PEB.EyeSolver.Network.Evolution;

namespace PEB.Network.Test
{
    /// <summary>
    /// Summary description for UTEvolution
    /// </summary>
    [TestClass]
    public class UTEvolution
    {
        //Dont use fill when NOT testing
        private Evolution net = new Evolution(100, 2, 3, 4, 5,0.5, 0.1);

        [TestMethod]
        public void Propigate()
        {
            if (net == null)
                throw new Exception("Reader is empty");
            Assert.AreEqual(100, net.Count);

            List<double> inputs = new List<double>(),
                goals = new List<double>();
            inputs.Add(0.5);
            inputs.Add(0.5);
            goals.Add(0.5);
            goals.Add(0.5);
            goals.Add(0.5);
            goals.Add(0.5);

            net.Propigate(inputs,goals);
            List<double> error = net.getError();
            net = net.Sort();
            double expected = net.getError().Last();

            for (int i = 0; i < 100; i++)
            {
                net.Mutate();
                net.Propigate(inputs, goals);
                net = net.Sort();

                double testing = net.Last().getError();

                Assert.AreNotEqual(0,testing);
            }
            double actual = net.Last().getError();
            Assert.IsTrue(expected > actual);
        }
    }
}
