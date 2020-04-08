#pragma once
#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#include "Map.h"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

const int nextblock_count = 5;

using namespace std;
namespace np = boost::python::numpy;

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

	boost::python::list transitions();

	np::ndarray get_screen();

private:
	vector<int> bag;
	int pop_block();
};

