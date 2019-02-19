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

        public Evolution evo = new Evolution(1000, 2, 2, 8, 2, 0.1);

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

        private async void CartesianChart_Loaded(object sender, RoutedEventArgs e)
        {
            while (true)
            {
                int count = 0;
                Task task =new Task(runNetwork);
                task.Start();
                await task;
                if (count % 4 == 0)
                {
                    //SeriesCollection[0].Values.RemoveAt(count / 3);
                    //SeriesCollection[1].Values.RemoveAt(count / 3);
                    //SeriesCollection[2].Values.RemoveAt(count / 3);
                }

                DataContext = this;
                count++;
            }
        }

        private void runNetwork()
        {

                List<double> inputs = new List<double>(),
                    outputs = new List<double>();
                inputs.Add(0.2);
                inputs.Add(0.5);
                outputs.Add(0.8);
                outputs.Add(0.4);
                evo.doEpoc(inputs, outputs, 0.5);

                SeriesCollection[0].Values.Add(evo.getError()[0]);
                SeriesCollection[1].Values.Add(evo.getError()[(evo.Count - 1) / 2]);
                SeriesCollection[2].Values.Add(evo.getError()[evo.Count - 1]);
        }
    }
}

  