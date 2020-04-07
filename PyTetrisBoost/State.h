#pragma once
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#include "Map.h"
const int nextblock_count = 5;

using namespace std;

class State {
public:
	State();
	State(const State& origin);
	Map map;
	vector<int> next_block; // index 0: falling block
	int hold;
	bool hold_used;
	int combo;
	bool btb;

	State use_hold();
	bool available(Pos pos);
	State put(Pos pos);

	vector<State> transitions();
private:
	vector<int> bag;
	int pop_block();
};

