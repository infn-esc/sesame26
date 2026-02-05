
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Step 1: Define a class representing a particle
class Particle {
public:
  // Step 1.1: Define getters and setters for the data members

  // Step 2: Write a method returning the energy of the particle
};

// Step 3: Write a function returning the distance between two particles
double distance(/* ... */) {}

PYBIND11_MODULE(HEP, m) {
  // Step 4: Bind the Particle class

  // Step 5: Bind the distance function
}
