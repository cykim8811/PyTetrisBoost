#pragma once
#include "State.h"


extern int wallkick_jlstz[4][2][5][2];
extern int wallkick_l[4][2][5][2];

vector<State> get_transitions(State& state);
