#pragma once
#include <algorithm>
#include "Block.h"
#include <iostream>

using namespace std;

typedef struct Pos {
	Pos(int _x, int _y, int _r)
		:x(_x), y(_y), r(_r) {}
	int x, y, r;
};

class Map {
public:
	const static int w = 10, h = 20;
	Map();
	bool available(int type, int x, int y, int r);
	bool available(int type, Pos pos);
	void put(int type, Pos pos);
	int data[Map::w * Map::h];
	int& at(int x, int y) { return data[x * Map::h + y]; }
};
