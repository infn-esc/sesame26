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
    // ...
    std::cout << "array: " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    // ...
    std::cout << "vector (without reserve): " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    // ...
    std::cout << "vector (with reserve): " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    // ...
    std::cout << "vector (sized): " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    // ...
    std::cout << "list: " << Duration{Clock::now() - start}.count() << " s\n";
  }

  {
    auto start = Clock::now();
    // ...
    std::cout << "list (sized): " << Duration{Clock::now() - start}.count() << " s\n";
  }
}
