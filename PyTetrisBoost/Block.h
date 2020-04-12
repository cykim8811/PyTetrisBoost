#pragma once
#include <vector>
using namespace std;
typedef struct Block {
	Block(int size, vector<int>);
	~Block();
	int size;
	int* data[4];
	int at(int x, int y, int r);
};

extern Block Shape[7];
extern int TileColor[8][3];
