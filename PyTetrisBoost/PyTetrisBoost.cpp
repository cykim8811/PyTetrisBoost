#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <iostream>

#include "Window.h"
#include "State.h"
#include "TreeAlg.h"

using namespace std;
using namespace boost::python;

BOOST_PYTHON_MODULE(PyTetrisBoost)
{
    np::initialize();

    class_<Window>("Window", init<>())
        .def("update", &Window::update)
        .def_readwrite("state", &Window::state)
        ;

    class_<Pos>("Pos", init<int, int, int>());

    class_<State>("State", init<>())
        .def_readwrite("hold_used", &State::hold_used)
        .def_readwrite("block_hold", &State::hold)
        .def_readwrite("combo", &State::combo)
        .def_readwrite("btb", &State::btb)
        .def_readwrite("dscore", &State::last_dscore)
        .def("next_block", &State::get_next_block)
        .def("available", &State::available)
        .def("use_hold", &State::use_hold)
        .def("put", &State::put)
        .def("transitions", &State::transitions)
        .def("screen", &State::get_screen)
        .def("set_screen", &State::set_screen)
        .def("compile", &State::compile)
        .def("compile_transitions", &State::compile_transitions)
        .def("add_trash", &State::add_trash)
        ;
}
