using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using LiveCharts;
using LiveCharts.Wpf;
using LiveCharts.Defaults; //Contains the already defined types

using PEB.EyeSolver.Network.Evolution;

namespace PEB.EyeSolver.UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        async Task PutTaskDelay()
        {
            await Task.Delay(5000);
        }

        public Evolution evo = new Evolution(1000,4, 4, 8, 2, 0.1);

        public MainWindow()
        {
            InitializeComponent();


            SeriesCollection = new SeriesCollection
            {
                new LineSeries
                {
                    Title = "Top Network",
                    Values = new ChartValues<double> { }
                },
                new LineSeries
                {
                    Title = "Median Network",
                    Values = new ChartValues<double> {  },
                    //PointGeometry = null
                },
                new LineSeries
                {
                    Title = "Worst Network",
                    Values = new ChartValues<double> { },
                    //PointGeometry = DefaultGeometries.Square,
                    //PointGeometrySize = 15
                }
            };

            //Labels = new[] { "Jan", "Feb", "Mar", "Apr", "May" };
            //YFormatter = value => value.ToString("C");

            //modifying the series collection will animate and update the chart
            //SeriesCollection.Add(new LineSeries
            //{
            //    Title = "Series 4",
            //    Values = new ChartValues<double> { 5, 3, 2, 4 },
            //    LineSmoothness = 0, //0: straight lines, 1: really smooth lines
            //    PointGeometry = Geometry.Parse("m 25 70.36218 20 -28 -20 22 -8 -6 z"),
            //    PointGeometrySize = 50,
            //    PointForeground = Brushes.Gray
            //});

            //modifying any series values will also animate and update the chart
            //SeriesCollection[3].Values.Add(5d);

            
        }

        public SeriesCollection SeriesCollection { get; set; }
        public string[] Labels { get; set; }
        public Func<double, string> YFormatter { get; set; }
        public bool graph = false;

        private async void CartesianChart_Loaded(object sender, RoutedEventArgs e)
        {
            long second = 0;
            int fps = 0;
            int count = 0;
            int divider = -1;


            DataContext = this;
            while (true)
            {

                if (count > divider)
                {
                    divider = (int) Math.Pow(count, 1.25);
                    Task<string> task = new Task<string>(runNetwork);
                    task.Start();
                    txbError.Text= await task;
                }

                    DataContext = this;

                count++;

                second += evo.getTime();
                fps++;
                if (second >= 1000)
                {
                    if (fps==1)
                    {
                        txbOut.Text = (second/1000).ToString()+" Seconds";
                        second = 0;
                    }
                    else
                    {
                        txbOut.Text = fps.ToString()+" per second";
                        second -= 1000;
                    }
                    fps = 0;

                }
            }
        }

        private string runNetwork()
        {
            List<double> inputs = new List<double>(),
                outputs = new List<double>();
            inputs.Add(0.2);
            inputs.Add(0.5);
            inputs.Add(0.6);
            inputs.Add(0.8);
            outputs.Add(0.8);
            outputs.Add(0.4);
            evo.doEpoc(inputs, outputs, 0.5);

            if (graph)
            {
                SeriesCollection[0].Values.Add(evo.getError()[0] * 1);
                SeriesCollection[1].Values.Add(evo.getError()[(evo.Count - 1) / 2] * 1);
                SeriesCollection[2].Values.Add(evo.getError()[evo.Count - 1] * 1);
            }
            else
            {
                return evo.getError()[0].ToString();
            }

            return "";
        }
    }
}

  