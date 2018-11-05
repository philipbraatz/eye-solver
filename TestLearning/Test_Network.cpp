#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestNet
{		
	TEST_CLASS(Test_Network)
	{
	public:
		TEST_METHOD(Test_Evolution)
		{
			EvoNet<Nnet> netGroup(10,0.1,3,3,3,3,problem_type::TEXT);

			bool max = true;
			bool start = true;
			double first,best;
			vector<double> solution = { 1,0,5,0.75 };
			//first run
			//				solution !test, maximize, !prune
			netGroup.DoEpoch(solution, false, max, false);
			netGroup.repopulate(0.5);
			netGroup.updateStats(max);
			first = netGroup.getBestScore();
	
			//loop run
			while (start == best)
			{

				netGroup.DoEpoch(solution, false, max, false);
				netGroup.repopulate(0.5);
				netGroup.updateStats(max);
				best = netGroup.getBestScore();
				if (netGroup.getGenCount() > 100)
				{
					Assert::Fail;
					break;
				}
			}
		}

	};
}