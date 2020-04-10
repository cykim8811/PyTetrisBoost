#include "TreeAlg.h"

Tree TreeAlg::generate(State state, int depth) {
	if (depth == 0) {
		vector<State> tr = state.transitions_vector();
		Tree ret{ true, {}, {}, state_list.size(), tr.size() };
		state_list.insert(state_list.end(), tr.begin(), tr.end());
		return ret;
	}
	else {
		vector<State> tr = state.transitions_vector();
		vector<Tree> tv;
		for (int i = 0; i < tr.size(); i++) {
			tv.push_back(generate(tr[i], depth - 1));
		}
		return Tree{ false, tv, tr };
	}
}

TreeAlg::TreeAlg(State state, int depth) {
	tree = generate(state, depth);
}


int& at(int* target, int i, int x, int y, int c) {
	return target[((i * Map::w + x) * Map::h + y) * channel_size + c];
}

np::ndarray TreeAlg::request() {
	int n = state_list.size();
	Py_intptr_t shape[4] = { n, Map::w, Map::h, channel_size };
	np::ndarray result = np::zeros(2, shape, np::dtype::get_builtin<int>());
	int* target = reinterpret_cast<int*>(result.get_data());

	for (int i = 0; i < n; i++) {
		for (int x = 0; x < Map::w; x++) {
			for (int y = 0; y < Map::h; y++) {
				at(target, i, x, y, 0) = state_list[i].map.at(x, y);
				for (int c = 0; c < 7; c++) {
					at(target, i, x, y, 1 + 7 * 0 + c) = (state_list[i].next_block[0] == c);
					at(target, i, x, y, 1 + 7 * 1 + c) = (!state_list[i].hold_used && state_list[i].hold == c);
					at(target, i, x, y, 1 + 7 * 2 + c) = (state_list[i].next_block[1] == c);
				}
				at(target, i, x, y, 22) = state_list[i].combo;
				at(target, i, x, y, 23) = state_list[i].btb;
			}
		}
	}

	return result;
}

void TreeAlg::fetch(np::ndarray result) {
	float* target = reinterpret_cast<float*>(result.get_data());
	
}

float calculate(Tree& tree) {

}