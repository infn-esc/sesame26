cmake_minimum_required(VERSION 3.16.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(pybind-HandsOn VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.13.6
)
FetchContent_MakeAvailable(pybind11)

# Playground for the examples
pybind11_add_module(Playground SHARED playground.cpp)

# Exercise 01
# pybind11_add_module(HEP SHARED 01_particle.cpp)

# Exercise 02
# pybind11_add_module(Kernels SHARED 02_kernels.cpp)

# Exercise 03
# pybind11_add_module(Matrix SHARED 03_matrix.cpp)

# Exercise 04
# add_executable(accumulate 04_list_accumulate.cpp)
# target_link_libraries(accumulate PRIVATE pybind11::embed)
# target_link_libraries(accumulate PRIVATE dl)
# target_link_libraries(accumulate PRIVATE pthread)
# target_link_libraries(accumulate PRIVATE util)

# Exercise 05
# pybind11_add_module(ArrayOperations SHARED 05_array_operations.cpp)
