/*
Copyright (C) 2026 Andrea Bocci
SPDX-License-Identifier: GNU General Public License v3.0 or later

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <numbers>
#include <random>
#include <vector>

#include <tbb/tbb.h>

template <typename T>
T compute(T x, T y) {
  auto a = std::sin(x);
  auto b = std::cos(y);
  auto c = std::sqrt(a * a - b * b);
  return c;
}

template <typename T>
void sequential_compute(std::vector<T> const& x, std::vector<T> const& y, std::vector<T>& z) {
  std::size_t size = x.size();
  for (std::size_t i = 0; i < size; ++i) {
    z[i] = compute(x[i], y[i]);
  }
}

template <typename T>
void parallel_compute(std::vector<T> const& x, std::vector<T> const& y, std::vector<T>& z) {
  std::size_t size = x.size();
  tbb::parallel_for<std::size_t>(0, size, 1, [&](std::size_t i) {
    z[i] = compute(x[i], y[i]);
  });
}

template <typename T>
void measure_sequential(std::vector<T> const& x, std::vector<T> const& y) {
  std::vector<T> z(x.size(), 0);
  auto start = std::chrono::steady_clock::now();
  sequential_compute(x, y, z);
  auto finish = std::chrono::steady_clock::now();
  float ms = std::chrono::duration_cast<std::chrono::duration<float>>(finish - start).count() * 1000.f;
  std::cout << std::format("{:6.1f}", ms) << " ms\n";
}

template <typename T>
void measure_parallel(std::vector<T> const& x, std::vector<T> const& y) {
  std::vector<T> z(x.size(), 0);
  auto start = std::chrono::steady_clock::now();
  parallel_compute(x, y, z);
  auto finish = std::chrono::steady_clock::now();
  float ms = std::chrono::duration_cast<std::chrono::duration<float>>(finish - start).count() * 1000.f;
  std::cout << std::format("{:6.1f}", ms) << " ms\n";
}

int main() {
  const std::size_t size = 10'000'000;
  const std::size_t repetitions = 10;

  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<float> dis{-std::numbers::pi, std::numbers::pi};
  std::vector<float> x(size);
  std::ranges::generate(x, [&] { return dis(gen); });
  std::vector<float> y(size);
  std::ranges::generate(y, [&] { return dis(gen); });

  std::cout << "sequential computation\n";
  for (size_t i = 0; i < repetitions; ++i)
    measure_sequential(x, y);
  std::cout << '\n';

  std::cout << "parallel computation\n";
  for (size_t i = 0; i < repetitions; ++i)
    measure_parallel(x, y);
  std::cout << '\n';
}
