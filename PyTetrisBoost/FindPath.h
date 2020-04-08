#pragma once
#include "State.h"
#include <boost/python/numpy.hpp>

namespace np = boost::python::numpy;

extern int wallkick_jlstz[4][2][5][2];
extern int wallkick_l[4][2][5][2];

vector<State> get_transitions(State& state);

void flood_fill(int* map, Map& scr, int type, Pos pos);
void flood_fill(int* map, Map& scr, int type, int x, int y, int r);