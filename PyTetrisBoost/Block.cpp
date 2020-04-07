#include "Block.h"

Block Shape[7] = {
    Block{4,
        {
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 1, 0, 0
        }
    },
    Block{3,
        {
        1, 1, 0,
        0, 1, 0,
        0, 1, 0
        }
    },
    Block{3,
        {
        0, 1, 0,
        0, 1, 0,
        1, 1, 0
        }
    },
    Block{2,
        {
        1, 1,
        1, 1
        }
    },
    Block{3,
        {
        0, 1, 0,
        1, 1, 0,
        1, 0, 0
        }
    },
    Block{3,
        {
        0, 1, 0,
        1, 1, 0,
        0, 1, 0
        }
    },
    Block{3,
        {
        1, 0, 0,
        1, 1, 0,
        0, 1, 0
        }
    },
};

int TileColor[7][3]{
    {48, 210, 230},
    {32, 48, 200},
    {230, 180, 48},
    {240, 240, 48},
    {32, 170, 72},
    {210, 64, 235},
    {200, 48, 42},
};

Block::Block(int _size, vector<int> _data) {
	size = _size;
	data[0] = new int[size * size];
	for (int i = 0; i < size * size; i++) {
		data[0][i] = _data[i];
	}
	for (int r = 0; r < 3; r++) {
		data[r + 1] = new int[size * size];
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				data[r + 1][y + x * size] = data[r][(size - 1 - x) + y * size];
			}
		}
	}
}

Block::~Block() {
	for (int i = 0; i < 4; i++) {
		delete[] data[i];
	}
}

int Block::at(int x, int y, int r) {
//    if (x < 0 || x >= size || y < 0 || y >= size)
//        return -1;
    return data[r][x * size + y];
}