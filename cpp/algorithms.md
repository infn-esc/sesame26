---
title: Use of algorithms and function objects
layout: main
section: cpp
---

The goal of these exercises is to get some confidence with the C++ entities that
are _callable_: functions, function objects and lambdas. You will use them to
customize the behaviour of some of the algorithms provided by the C++ standard
library.

## Exercise 1

Take the solution for [`containers.cpp`]({% include hands_on_base.md %}/{{ page.section }}/containers.cpp)
and replace the existing loops with appropriate algorithms, e.g. `std::fill` and
`std::fill_n`, possibly using a `std::back_insert_iterator`.

[cppreference.com](http://en.cppreference.com/w/cpp/algorithm) is an excellent
source of documentation for the algorithms (and C++ in general).

## Exercise 2

Take [`containers_generic.cpp`]({% include hands_on_base.md %}/{{ page.section
}}/containers_generic.cpp). It implements for a `std::vector<int>` two operations:

* `fill()`, which fills the container with `N` integers randomly
generated. Each integer is inserted in a random position.

* `process()`, which applies a reduction operation to the
previously-filled container.

Inspect, build and run the program, varying the number of elements:

```shell
$ cd {{ site.github.repository_name }}/hands-on/{{ page.section }}
$ g++ -Wall -Wextra -O3 -o containers_generic containers_generic.cpp
$ ./containers_generic 100000
vector fill: 0.526868 s
vector process: 2.9868e-05 s
```

Then extend the program to implement `fill()` and `process()` for a
`std::list<int>` (Hint: note that in both functions there is nothing specific to `std::vector`).
Compare the performance obtained with the two containers.

## Exercise 3

`std::default_random_engine` usually is an alias for a [_linear congruential_
generator](https://en.wikipedia.org/wiki/Linear_congruential_generator). Let's
consider _minstd_rand0_, which produces a sequence according to

$$
x_{n+1} = 16807 x_n \mod (2^{31} - 1)
$$

Starting from [`linear_congruential.cpp`]({% include hands_on_base.md %}/{{ page.section
}}/linear_congruential.cpp), write a class `LinearCongruential` whose constructor initializes the sequence
with a seed (with a default value of 1) and an `operator()` that updates the
internal value (the $x_n$) and returns it. The type of the numbers involved in
the computations is `unsigned long int` (Hint: $2^{31}$ can be easily represented as `1ul << 31`).

Print a few numbers and check that they correspond to what is produced by
`std::default_random_engine`.

## Exercise 4

* Given a short vector of small ints

  * compute the product of the numbers (Hint: use
`std::accumulate`). Be careful with the overflow
  * sort it in decresing order (Hint: use `std::sort` with an
appropriate comparison function)
  * compute the mean and the standard deviation in one pass (Hint:
use `std::accumulate`, accumulating the sum and the sum of
squares in a `struct`)

* Generate N numbers, using one of the available random number
distributions, and insert them in a `std::vector` (Hint: use
`std::generate_n`)

For the callables to pass to the algorithms, try with plain functions, function objects and lambda expressions.
