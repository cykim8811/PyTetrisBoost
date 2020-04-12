
#include "State.h"
#include "FindPath.h"
#include <array>


static int combo_score[12] = {
	0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4
};
static int clear_score[5] = {
	0, 0, 1, 2, 4
};

template <class T>
boost::python::list toPythonList(std::vector<T> vector) {
	typename std::vector<T>::iterator iter;
	boost::python::list list;
	for (iter = vector.begin(); iter != vector.end(); ++iter) {
		list.append(*iter);
	}
	return list;
}

State::State() {
	hold = -1;
	hold_used = false;

	combo = 0;
	btb = false;
	next_block.reserve(8);
	for (int i = 0; i < nextblock_count; i++) {
		next_block.push_back(pop_block());
	}
	last_dscore = 0;
}

State::State(const State& origin) {
	hold = origin.hold;
	hold_used = origin.hold_used;

	combo = origin.combo;
	btb = origin.btb;
	next_block = origin.next_block;
	bag = origin.bag;
	for (int i = 0; i < Map::w * Map::h; i++) {
		map.data[i] = origin.map.data[i];
	}
	last_dscore = origin.last_dscore;
}

int State::pop_block() {
	if (bag.size() == 0) {
		srand((unsigned int)time(0));
		for (int i = 0; i < 7; i++)
			bag.push_back(i);
		random_shuffle(bag.begin(), bag.end());
	}
	int ret = bag.back();
	bag.pop_back();
	return ret;
}

State State::use_hold() {
	State ret(*this);
	ret.hold_used = true;
	if (ret.hold == -1) {
		ret.hold = ret.next_block[0];
		ret.next_block.erase(ret.next_block.begin());
		ret.next_block.push_back(ret.pop_block());
	}
	else {
		int tmp = ret.next_block[0];
		ret.next_block[0] = ret.hold;
		ret.hold = tmp;
	}
	return ret;
}

bool State::available(Pos pos) {
	return map.available(next_block[0], pos);
}

State State::put(Pos pos) {
	State ret(*this);
	ret.last_dscore = 0;
	if (ret.map.available(ret.next_block[0], pos)) {
		ret.map.put(ret.next_block[0], pos);
	}
	else {
		// When dead
		State rett;
		rett.last_dscore = -100;
		return rett;
	}
	ret.hold_used = false;

	bool tspin = false;
	// T-spin detection
	if (next_block[0] == 5) {
		int empty_count = 0;
		if (pos.y >= 18 || pos.x < 0 || map.at(pos.x, pos.y + 2))
			empty_count++;
		if (pos.y >= 18 || pos.x >= 8 || map.at(pos.x + 2, pos.y + 2))
			empty_count++;
		if (pos.x < 0 || map.at(pos.x, pos.y))
			empty_count++;
		if (pos.x >= 8 || map.at(pos.x + 2, pos.y))
			empty_count++;

		if (empty_count >= 3 && !map.available(5, Pos{ pos.x, pos.y - 1, pos.r })) {
			tspin = true;
		}
	}
	
	// Clear Lines
	int clear_count = 0;
	for (int y = 19; y >= 0; y--) {
		bool all = true;
		for (int x = 0; x < 10; x++) {
			if (ret.map.at(x, y) == 0) {
				all = false;
				break;
			}
		}
		if (all) {
			for (int x = 0; x < 10; x++) {
				for (int cy = y; cy > 0; cy--) {
					ret.map.at(x, cy) = ret.map.at(x, cy - 1);
				}
				ret.map.at(x, 0) = 0;
			}
			clear_count++;
			y += 1;
		}
	}

	if (clear_count > 0) {
		ret.last_dscore += clear_score[clear_count];
		ret.combo += 1;
		ret.last_dscore += combo_score[ret.combo];

		if (tspin || clear_count == 4) {
			if (ret.btb) ret.last_dscore += 1;
			ret.btb = true;
		}
		else {
			ret.btb = false;
		}
		if (tspin) {
			ret.last_dscore += clear_count + 1;
		}
		bool all = true;
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 10; x++) {
				if (ret.map.at(x, y) != 0) {
					all = false;
					break;
				}
			}
			if (all == false) break;
		}
		if (all) {
			ret.last_dscore += 10;
		}
	}
	else {
		ret.combo = 0;
	}

	ret.next_block.push_back(ret.pop_block());
	ret.next_block.erase(ret.next_block.begin());
	return ret;
}

boost::python::list State::transitions() {
	return toPythonList(get_transitions(*this));
}

vector<State> State::transitions_vector() {
	return get_transitions(*this);
}

boost::python::list State::get_next_block() {
	return toPythonList(next_block);
}


np::ndarray State::get_screen() {
	Py_intptr_t shape[2] = { Map::w, Map::h };
	np::ndarray result = np::zeros(2, shape, np::dtype::get_builtin<int>());
	copy(begin(map.data), end(map.data), reinterpret_cast<int*>(result.get_data()));
	return result;
}

void State::set_screen(np::ndarray target) {
	int* target_pointer = reinterpret_cast<int*>(target.get_data());
	for (int i = 0; i < 200; i++) {
		map.data[i] = target_pointer[i];
	}
}

/* Format:
Map[0]		1		map of S_0
Next		7		next block one-hot
Hold		7		hold block one-hot
Next[2]		7		nnext block one-hot
combo		1		current combo int
btb			1		back-to-back bool
Map[1]		1		map of S_1
*/
np::ndarray State::compile(State state) {
	Py_intptr_t shape[3] = { Map::w, Map::h, 25};
	np::ndarray result = np::zeros(3, shape, np::dtype::get_builtin<int>());
	int* target = reinterpret_cast<int*>(result.get_data());
	for (int x = 0; x < Map::w; x++) {
		for (int y = 0; y < Map::h; y++) {
			target[(x * Map::h + y) * 25 + 0] = (map.at(x, y) != 0);
			for (int c = 0; c < 7; c++) {
				target[(x * Map::h + y) * 25 + 1 + 7 * 0 + c] = (next_block[0] == c);
			}
			for (int c = 0; c < 7; c++) {
				target[(x * Map::h + y) * 25 + 1 + 7 * 1 + c] = (hold == c);
			}
			for (int c = 0; c < 7; c++) {
				target[(x * Map::h + y) * 25 + 1 + 7 * 2 + c] = (next_block[1] == c);
			}
			target[(x * Map::h + y) * 25 + 22] = combo;
			target[(x * Map::h + y) * 25 + 23] = btb;
			target[(x * Map::h + y) * 25 + 24] = (state.map.at(x, y) != 0);
		}
	}
	return result;
}

np::ndarray State::compile_transitions() {
	vector<State> trans = transitions_vector();
	Py_intptr_t shape[4] = { trans.size(), Map::w, Map::h, 25 };
	np::ndarray result = np::zeros(4, shape, np::dtype::get_builtin<int>());
	int* target = reinterpret_cast<int*>(result.get_data());
	for (int t = 0; t < trans.size(); t++) {
		for (int x = 0; x < Map::w; x++) {
			for (int y = 0; y < Map::h; y++) {
				int offset = ((t * Map::w + x) * Map::h + y) * 25;
				target[offset + 0] = (map.at(x, y) != 0);
				for (int c = 0; c < 7; c++) {
					target[offset + 1 + 7 * 0 + c] = (next_block[0] == c);
				}
				for (int c = 0; c < 7; c++) {
					target[offset + 1 + 7 * 1 + c] = (hold == c);
				}
				for (int c = 0; c < 7; c++) {
					target[offset + 1 + 7 * 2 + c] = (next_block[1] == c);
				}
				target[offset + 22] = combo;
				target[offset + 23] = btb;
				target[offset + 24] = (trans[t].map.at(x, y) != 0);
			}
		}
	}
	return result;
}
