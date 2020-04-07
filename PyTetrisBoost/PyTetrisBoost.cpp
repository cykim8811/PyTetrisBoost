#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <iostream>

using namespace std;
using namespace boost::python;

BOOST_PYTHON_MODULE(PyTetrisBoost)
{
    class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
        ;
}