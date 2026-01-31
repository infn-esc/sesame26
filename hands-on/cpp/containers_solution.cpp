#include <array>
#include <vector>
#include <list>
#include <chrono>
#include <iostream>

using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<float>;

int main()
{
  constexpr auto N = 1'000'000'000;
  {
    auto start = Clock::now();
    std::array<int, N/1000> c;
    for (int& e : c) {
      e = 1234;
    }
    std::cout << "array: " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    std::vector<int> c;
    for (int i{0}; i != N; ++i) {
      c.push_back(1234);
    }
    std::cout << "vector (without reserve): " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    std::vector<int> c;
    c.reserve(N);
    for (int i{0}; i != N; ++i) {
      c.push_back(1234);
    }
    std::cout << "vector (with reserve): " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    std::vector<int> c(N);
    for (int& e : c) {
      e = 1234;
    }
    std::cout << "vector (sized): " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    std::list<int> c;
    for (int i{0}; i != N; ++i) {
      c.push_back(1234);
    }
    std::cout << "list: " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    std::list<int> c(N);
    for (int& e : c) {
      e = 1234;
    }
    std::cout << "list (sized): " << Duration{Clock::now() - start}.count() << " s\n";
  }
}
