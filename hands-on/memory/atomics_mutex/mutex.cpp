#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void count_even_odd(const std::vector<int> &x, int &even_count,
                    int &odd_count) {
  const std::size_t N = x.size();
  const unsigned n_threads = 4;

  even_count = 0;
  odd_count = 0;

  std::vector<std::thread> threads;
  threads.reserve(n_threads);

  // TODO: Implement worker function
  auto worker = [&](std::size_t begin, std::size_t end) {
    // TODO
  };

  // Split work into chunks
  std::size_t chunk = -1; // FIXME!
  for (unsigned t = 0; t < n_threads; ++t) {

    std::size_t begin = -1; // FIXME!;
    std::size_t end = -1;   // FIXME!;
    if (begin < eend)
      threads.emplace_back(worker, begin, end);
  }

  // Join all threads
  for (auto &th : threads)
    th.join();
}

int main() {
  std::vector<int> x(100);
  for (int i = 0; i < 100; ++i)
    x[i] = i;

  int even = 0, odd = 0;
  count_even_odd(x, even, odd);

  std::cout << "Even: " << even << " Odd: " << odd << "\n";
}
