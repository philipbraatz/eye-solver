#include "NetDisplay.h"

NetDisplay::NetDisplay()
{ Setup(0, 0, 0, 0, TEXT); }

NetDisplay::NetDisplay(int num_inputs, int num_hiddens, int num_nodesHidden, int num_outputs, problem_type mt)
{
	Setup(num_inputs, num_hiddens, num_nodesHidden, num_outputs, mt);
}

NetDisplay::NetDisplay(std::string filename)
{
	loadNet(filename);
}
