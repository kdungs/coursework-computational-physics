#include <boost/python.hpp>
#include "union_find.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(unionfind) {
  class_<UnionFind>("UnionFind")
    .def("make_set", &UnionFind::MakeSet)
    .def("union", &UnionFind::Union)
    .def("find", &UnionFind::Find);
}
