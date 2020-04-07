#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <iostream>

#include "Window.h"
#include "State.h"

using namespace std;
using namespace boost::python;

BOOST_PYTHON_MODULE(PyTetrisBoost)
{
    class_<Window>("Window", init<>())
        .def("update", &Window::update)
        .def_readwrite("state", &Window::state)
        ;

    class_<Pos>("Pos", init<int, int, int>());

    class_<State>("State", init<>())
        .def_readwrite("hold_used", &State::hold_used)
        .def("available", &State::available)
        .def("use_hold", &State::use_hold)
        .def("put", &State::put)
        ;
}