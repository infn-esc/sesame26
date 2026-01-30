#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

// utility class for timing
class Timer {
public:
  Timer() : start_time_(std::chrono::high_resolution_clock::now()) {}

  void reset() { start_time_ = std::chrono::high_resolution_clock::now(); }

  double elapsed() const {
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time =
        end_time - start_time_;
    return elapsed_time.count();
  }

private:
  std::chrono::high_resolution_clock::time_point start_time_;
};

void fill_random(std::vector<double> &M, int N) {
  std::mt19937_64 rng(42);
  std::uniform_real_distribution<double> dist(-1.0, 1.0);
  for (auto &x : M)
    x = dist(rng);
}

void zero(std::vector<double> &M) { std::fill(M.begin(), M.end(), 0.0); }

// C = A * B  (no transpose).
void matmul_naive(const std::vector<double> &A, const std::vector<double> &B,
                  std::vector<double> &C, int N) {
  zero(C);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      double sum = 0.0;
      const int iN = i * N;
      for (int k = 0; k < N; ++k) {
        sum += A[iN + k] * B[k * N + j];
      }
      C[iN + j] = sum;
    }
  }
}

void matmul_with_transpose(const std::vector<double> &A,
                           const std::vector<double> &B, std::vector<double> &C,
                           int N) {
  // transposing
  std::vector<double> Bt(N * N);
  for (int i = 0; i < N; ++i) {
    const int iN = i * N;
    for (int j = 0; j < N; ++j) {
      Bt[iN + j] = B[j * N + i];
    }
  }
  // Multiply using Bt
  zero(C);
  for (int i = 0; i < N; ++i) {
    const int iN = i * N;
    for (int j = 0; j < N; ++j) {
      const int jN = j * N;
      double sum = 0.0;
      for (int k = 0; k < N; ++k) {
        sum += A[iN + k] * Bt[jN + k];
      }
      C[iN + j] = sum;
    }
  }
}

int main(int argc, char **argv) {
  int N = 1024;
  int repeats = 3;

  std::vector<double> A(N * N), B(N * N), C1(N * N), C2(N * N);
  fill_random(A, N);
  fill_random(B, N);

  double t_naive = 0.0;
  Timer timer;

  for (int r = 0; r < repeats; ++r) {
    timer.reset();
    matmul_naive(A, B, C1, N);
    t_naive += timer.elapsed();
  }
  t_naive /= repeats;

  double t_transpost = 0.0;
  for (int r = 0; r < repeats; ++r) {
    timer.reset();
    matmul_with_transpose(A, B, C2, N);
    t_transpost += timer.elapsed();
  }
  t_transpost /= repeats;

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Naive time:      " << t_naive << " ms\n";
  std::cout << "Transpose time:  " << t_transpost << " ms\n";
  std::cout << "Speedup (naive/trans): " << (t_naive / t_transpost) << "x\n";

  return 0;
}
