#include <atomic>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

// compile with -O3
// FIX THIS CODE!
int main() {
  const std::size_t N = 1000000;
  std::vector<int> data(N, 0);

  // Producer: fill data, then publish the data
  auto producer = [&] {
    for (std::size_t i = 0; i < N; ++i) {
      data[i] = i;
    }
  };

  auto consumer = [&] {
    long long s = 0;
    for (std::size_t i = 0; i < N; ++i) {
      s += data[i];
    }

    // Expected sum if data[i] = i
    long long expected = static_cast<long long>(N) * (N - 1) / 2;
    std::cout << "Sum=" << s << " expected=" << expected << std::endl;
    if (s == expected) {
      std::cout << "CORRECT! " << std::endl;
    } else {
      std::cout << "Try Again! " << std::endl;
    }
  };

  std::thread tP(producer);
  std::thread tC(consumer);
  tP.join();
  tC.join();
}
