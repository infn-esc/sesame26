
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Use this file to test the examples that you'll see during the lectures
PYBIND11_MODULE(Playground, m) {
	m.doc() = "Playground module for pybind11 examples";
}
