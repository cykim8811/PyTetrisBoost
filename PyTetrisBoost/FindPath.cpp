#include "FindPath.h"


// r_from, spin_dir, tests, dim
static int wallkick_jlstz[4][2][5][2]{
    {
        {{ 0, 0}, {-1, 0}, {-1, 1}, { 0,-2}, {-1,-2}},
        {{ 0, 0}, { 1, 0}, { 1, 1}, { 0,-2}, { 1,-2}},
    },
    {
        {{ 0, 0}, { 1, 0}, { 1,-1}, { 0, 2}, { 1, 2}},
        {{ 0, 0}, { 1, 0}, { 1,-1}, { 0, 2}, { 1, 2}}
    },
    {
        {{ 0, 0}, { 1, 0}, { 1, 1}, { 0,-2}, { 1,-2}},
        {{ 0, 0}, {-1, 0}, {-1, 1}, { 0,-2}, {-1,-2}}
    },
    {
        {{ 0, 0}, {-1, 0}, {-1,-1}, { 0, 2}, {-1, 2}},
        {{ 0, 0}, {-1, 0}, {-1,-1}, { 0, 2}, {-1, 2}}
    }
};
static int wallkick_l[4][2][5][2]{
    {
        {{ 0, 0}, {-2, 0}, { 1, 0}, {-2,-1}, { 1, 2}},
        {{ 0, 0}, {-1, 0}, { 2, 0}, {-1, 2}, { 2,-1}},
    },
    {
        {{ 0, 0}, {-1, 0}, { 2, 0}, {-1, 2}, { 2,-1}},
        {{ 0, 0}, { 2, 0}, {-1, 0}, { 2, 1}, {-1,-2}},
    },
    {
        {{ 0, 0}, { 2, 0}, {-1, 0}, { 2, 1}, {-1,-2}},
        {{ 0, 0}, { 1, 0}, {-2, 0}, { 1,-2}, {-2, 1}},
    },
    {
        {{ 0, 0}, { 1, 0}, {-2, 0}, { 1,-2}, {-2, 1}},
        {{ 0, 0}, {-2, 0}, { 1, 0}, {-2,-1}, { 1, 2}},
    }
};

void flood_fill(int* map, int x, int y, int r) {
    if (x < 0 || x >= Map::w || y < 0 || y >= Map::h + 2 || map[x * (Map::w * (Map::h + 2)) + y * Map::w + r] != 0)
        return;
    map[x * (Map::w * (Map::h + 2)) + y * Map::w + r] = 2;
    flood_fill(map, x + 1, y, r);
    flood_fill(map, x - 1, y, r);
    flood_fill(map, x, y + 1, r);
    flood_fill(map, x, y, (r + 1) % 4);
    flood_fill(map, x, y, (r + 3) % 4);
}

vector<State> get_transitions(State& state) {
    vector<State> ret;
    ret.push_back(state.use_hold());

    Block& block = Shape[state.next_block[0]];

    int freemap[Map::w * (Map::h + 2) * 4] = { 0 };

    for (int bx = 0; bx < block.size; bx++) {
        for (int by = 0; by < block.size; by++) {
            for (int br = 0; br < 4; br++) {
                if (!block.at(bx, by, br)) continue;
                for (int mx = 0; mx < Map::w; mx++) {
                    for (int my = 0; my < Map::h + 2; my++) {
                        int tx = mx - 2 + bx,
                            ty = my - 4 + by;
                        if (ty < 0) continue;
                        if (tx < 0 || tx >= Map::w || ty >= Map::h || state.map.at(tx, ty)) {
                            freemap[mx * (Map::w * (Map::h + 2)) + my * Map::w + br] = 1;
                        }
                    }
                }
            }
        }
    }

    flood_fill(&freemap[0], 5, 0, 0);


    for (int br = 0; br < 4; br++) {
        for (int mx = 0; mx < Map::w; mx++) {
            int prev = 2;
            for (int my = 0; my < Map::h + 2; my++) {
                int current = freemap[mx * (Map::w * (Map::h + 2)) + my * Map::w + br];
                if (prev == 2 && current == 1) {
                    ret.push_back(state.put(Pos{ mx - 2, my - 4, br }));
                }
                prev = current;
            }
        }
    }
    return ret;
}