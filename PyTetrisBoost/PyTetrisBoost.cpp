#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>
#include <iostream>

using namespace std;
using namespace boost::python;

char const* greet()
{
    return "hello, world";
}
struct World
{
    void set(std::string msg) { this->msg = msg; }
    std::string greet() { return msg; }
    std::string msg;
};

BOOST_PYTHON_MODULE(PyTetrisBoost)
{
    class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
        ;
}