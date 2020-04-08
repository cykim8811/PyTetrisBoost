
#include "State.h"
#include "FindPath.h"
#include <array>

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
}

int State::pop_block() {
	if (bag.size() == 0) {
		srand(unsigned(time(0)));
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
	ret.map.put(ret.next_block[0], pos);
	ret.next_block.push_back(ret.pop_block());
	ret.next_block.erase(ret.next_block.begin());
	return ret;
}

boost::python::list State::transitions() {
	return toPythonList(get_transitions(*this));
}



np::ndarray State::get_screen() {
	Py_intptr_t shape[2] = { Map::w, Map::h };
	np::ndarray result = np::zeros(2, shape, np::dtype::get_builtin<int>());
	copy(begin(map.data), end(map.data), reinterpret_cast<int*>(result.get_data()));
	return result;
}