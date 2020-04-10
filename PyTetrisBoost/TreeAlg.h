#pragma once
#include "State.h"

const int channel_size = 24;

typedef struct Tree {
	bool isleaf;
	vector<Tree> branches;
	vector<State> owner;
	int offset;
	int size;
};

float calculate(Tree& tree);

class TreeAlg {
public:
	TreeAlg(State state, int depth);
	Tree generate(State state, int depth);
	np::ndarray request();
	void fetch(np::ndarray result);
	State new_state();
	np::ndarray X();
	np::ndarray Y();
private:
	vector<State> state_list;
	Tree tree;
};

