#pragma once
#include <cstdlib>

#include <fstream>
#include <iostream>//DEBUG

float RandNum() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }