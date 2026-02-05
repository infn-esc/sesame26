---
title: Binding Python and C++ with pybind11
layout: main
section: pybind
---

## Exercise 1: Particle class
This first exercise is meant to get comfortable with pybind11's syntax.
Define a class `Particle` with the following attributes:
- `name` (const std::string)
- `x`, `y`, `z` coordinates (double)
- `px`, `py`, `pz` momentum (double)
- `mass` in GeV (double)
For this class write the necessary constructors, getters and setters, as well as a methods
returning the particle's energy.
Also define a function that returns the distance between two particles.

Finally bind all the code in a Python module and test it in Python.

Hint: `E = sqrt(px^2 + py^2 + pz^2 + mass^2)`

## Exercise 2: Matrix
Define a class `Matrix`, with the getters for the sizes and the data pointer, as well as
the two overloads of the `operator[]`. Then overload all the main arithmetic operators:
- addition and subtraction of two matrices
- in-place addition and subtraction
- multiplication of a matrix by a scalar (both commutations)
- division of a matrix by a scalar
- in-place multiplication and division by a scalar
- matrix-matrix multiplication

Then bind the class, defining all the arithmetic operators, the `__len__` and `__str__`
variables and the `__getitem__` and `__setitem__` methods.
