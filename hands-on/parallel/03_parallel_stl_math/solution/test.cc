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
#include <execution>
#include <format>
#include <iostream>
#include <numbers>
#include <random>
#include <vector>

template <typename T>
T kernel(T x, T y) {
  auto a = std::sin(x);
  auto b = std::cos(y);
  auto c = std::sqrt(a * a - b * b);
  return c;
}

template <typename T>
void measure(auto policy, std::vector<T> const& a, std::vector<T> const& b) {
  std::vector<T> c(a.size(), 0);
  auto start = std::chrono::steady_clock::now();
  std::transform(policy, a.begin(), a.end(), b.begin(), c.begin(), kernel<T>);
  auto finish = std::chrono::steady_clock::now();
  float ms = std::chrono::duration_cast<std::chrono::duration<float>>(finish - start).count() * 1000.f;
  std::cout << std::format("{:6.1f}", ms) << " ms\n";
};

int main() {
  const std::size_t size = 10'000'000;
  const std::size_t repetitions = 10;

  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<float> dis{-std::numbers::pi, std::numbers::pi};
  std::vector<float> a(size);
  std::ranges::generate(a, [&] { return dis(gen); });
  std::vector<float> b(size);
  std::ranges::generate(b, [&] { return dis(gen); });

  std::cout << "std::execution::seq\n";
  for (size_t i = 0; i < repetitions; ++i)
    measure(std::execution::seq, a, b);
  std::cout << '\n';

  std::cout << "std::execution::unseq\n";
  for (size_t i = 0; i < repetitions; ++i)
    measure(std::execution::unseq, a, b);
  std::cout << '\n';

  std::cout << "std::execution::par\n";
  for (size_t i = 0; i < repetitions; ++i)
    measure(std::execution::par, a, b);
  std::cout << '\n';

  std::cout << "std::execution::par_unseq\n";
  for (size_t i = 0; i < repetitions; ++i)
    measure(std::execution::par_unseq, a, b);
  std::cout << '\n';
}
