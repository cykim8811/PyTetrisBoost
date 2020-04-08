#include "Map.h"

Map::Map() {
	for (int i = 0; i < Map::w * Map::h; i++) {
		data[i] = 0;
	}
}

bool Map::available(int type, int x, int y, int r) {
	return Map::available(type, Pos{ x, y, r });
}

bool Map::available(int type, Pos pos) {
	Block& block = Shape[type];
	for (int x = 0; x < block.size; x++) {
		for (int y = 0; y < block.size; y++) {
			if (!block.at(x, y, pos.r))
				continue;
			int tx = pos.x + x,
				ty = pos.y + y;
			if (tx < 0 || tx >= Map::w || ty >= Map::h)
				return false;
			if (ty < 0)
				continue;
			if (data[tx * Map::h + ty])
				return false;
		}
	}
	return true;
}

void Map::put(int type, Pos pos) {
	Block& block = Shape[type];
	for (int x = 0; x < block.size; x++) {
		for (int y = 0; y < block.size; y++) {
			if (!block.at(x, y, pos.r))
				continue;
			int tx = pos.x + x,
				ty = pos.y + y;
			if (tx < 0 || tx >= Map::w || ty < 0 || ty >= Map::h)
				continue;
			data[tx * Map::h + ty] = block.at(x, y, pos.r) * (type + 1);
		}
	}
}
