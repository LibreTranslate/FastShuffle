#include <pybind11/pybind11.h>
#include "shuffle.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(fastshuffle, m) {
    m.def("file_shuffle", &shuffle);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}