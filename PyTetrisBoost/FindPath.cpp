#include "FindPath.h"

const int 
    off_x = 2,
    off_y = 4,
    off_w = 1,
    off_h = 3;

int& get_map(int* map, int x, int y, int r) {
    return map[(x * (Map::h + off_h) + y) * 4 + r];
}

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


Pos rotate(Map& map, int type, Pos pos, int n) {
    int rp = pos.r + n;

    while (rp < 0) rp += 4;
    rp = rp % 4;

    int dirind;
    if (rp == 1)
        dirind = 0;
    else
        dirind = 1;

    if (type == 3)
        return Pos{ pos.x, pos.y, rp };
    else if (type == 0) {
        for (int i = 0; i < 5; i++) {
            Pos cp{ pos.x + wallkick_l[pos.r][dirind][i][0], pos.y - wallkick_l[pos.r][dirind][i][1], rp % 4 };
            if (map.available(type, cp)) {
                return cp;
            }
        }
    }
    else {
        for (int i = 0; i < 5; i++) {
            Pos cp{ pos.x + wallkick_jlstz[pos.r][dirind][i][0], pos.y - wallkick_jlstz[pos.r][dirind][i][1], rp % 4 };
            if (map.available(type, cp)) {
                return cp;
            }
        }
    }
    return Pos{ 0, 0, -1 };
}

inline void flood_fill(int* map, Map& scr, int type, Pos pos) {
    flood_fill(map, scr, type, pos.x, pos.y, pos.r);
}

void flood_fill(int* map, Map& scr, int type, int x, int y, int r) {
    if (x < 0 || x >= Map::w + off_w || y < 0 || y >= Map::h + off_h || get_map(map, x, y, r) != 0)
        return;
    get_map(map, x, y, r) = 2;
    flood_fill(map, scr, type, x + 1, y, r);
    flood_fill(map, scr, type, x - 1, y, r);
    flood_fill(map, scr, type, x, y + 1, r);
    flood_fill(map, scr, type, rotate(scr, type, Pos{ x, y, r }, 1));
    flood_fill(map, scr, type, rotate(scr, type, Pos{ x, y, r }, 1));
}

vector<State> get_transitions(State& state) {
    vector<State> ret;
    ret.push_back(state.use_hold());

    Block& block = Shape[state.next_block[0]];
    int freemap[(Map::w + off_w) * (Map::h + off_h) * 4] = { 0 };

    for (int br = 0; br < 4; br++) {
        for (int bx = 0; bx < block.size; bx++) {
            for (int by = 0; by < block.size; by++) {
                if (!block.at(bx, by, br)) continue;
                for (int mx = 0; mx < Map::w + off_w; mx++) {
                    for (int my = 0; my < Map::h + off_h; my++) {
                        int tx = mx - off_x + bx,
                            ty = my - off_y + by;
                        if (tx < 0 || tx >= Map::w || ty >= Map::h) {
                            get_map(freemap, mx, my, br) = 1;
                            continue;
                        }
                        if (ty < 0) continue;
                        if (state.map.at(tx, ty)) {
                            get_map(freemap, mx, my, br) = 1;
                            continue;
                        }
                    }
                }
            }
        }
    }



    flood_fill(&freemap[0], state.map, state.next_block[0], 5, 0, 0);


    for (int br = 0; br < 4; br++) {
        for (int mx = 0; mx < Map::w + off_w; mx++) {
            int prev = 1;
            for (int my = 0; my < Map::h + off_h; my++) {
                int current = get_map(freemap, mx, my, br);
                if (prev == 2 && current == 1) {
                    ret.push_back(state.put(Pos{ mx - 2, my - 4 - 1, br }));
                }
                prev = current;
            }
            if (prev == 2) {
                ret.push_back(state.put(Pos{ mx - off_x, (Map::h + off_h) - off_y - 1, br }));
            }
        }
    }
    return ret;
}
/*
    for (int br = 0; br < 4; br++) {
        for (int mx = 0; mx < Map::w + off_w; mx++) {
            for (int my = 0; my < Map::h + off_h; my++) {
                if (state.map.available(state.next_block[0], Pos{ mx - off_x, my - off_y, br })) {
                    get_map(freemap, mx, my, br) = 0;
                }
                else {
                    get_map(freemap, mx, my, br) = 1;
                }
            }
        }
    }
*/
