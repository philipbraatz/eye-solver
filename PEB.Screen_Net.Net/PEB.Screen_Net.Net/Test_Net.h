#include "EvoNet.h"

bool test_net()
{
	int genMax = 100000;
	int inprovements = 0;

	EvoNet<Nnet> netGroup(100,0.1, 3, 3, 3, 3, problem_type::TEXT);

	bool max = true;
	bool start = true;
	double previous,best;
	vector<double> solution = { 1,0,0.75 };
	//first run
	//				solution !test, maximize, !prune
	netGroup.DoEpoch(solution, false, max, false);
	netGroup.repopulate(0.5);
	netGroup.updateStats(max);
	previous = netGroup.getBestScore();
	best = netGroup.getBestScore();

	//loop run
	int i = 0;
	while (genMax > i)
	{
		netGroup.DoEpoch(solution, false, max, false);
		netGroup.repopulate(0.5);
		netGroup.updateStats(max);
		if (netGroup.getBestScore() > previous)
		{
			best = netGroup.getBestScore();
			inprovements++;
		}
		previous = netGroup.getBestScore();

		i++;
	}
	std::cout << endl << "Successful Learning Rate: %" << (double)inprovements / (double)genMax *100 << endl;
	return true;
}