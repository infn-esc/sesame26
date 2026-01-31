---
title: Containers
layout: main
section: cpp
---

The goal of this exercise is to appreciate the importance of
manipulating data structures that are contiguous in memory.

* Starting from [`containers.cpp`]({% include hands_on_base.md %}/{{ page.section
}}/containers.cpp), fill a [`std::array`](https://en.cppreference.com/w/cpp/container/array.html), a
[`std::vector`](https://en.cppreference.com/w/cpp/container/vector.html) and a [`std::list`](https://en.cppreference.com/w/cpp/container/list.html) with 1’000’000’000 numbers,
measuring the time it takes for each
* For `std::vector` try with and without calling `reserve()`
* For `std::vector` and `std::list` try with a container already
constructed with the right size
* For `std::array` the operation can fail badly. Why?
