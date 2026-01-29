// ─────────────────────────────────────────────────────────────────────────────
// Exercise 04 – Block-Level Reduction  (STUDENT VERSION)
//
// Goal:
//   • Compute the sum of a large 1‑D array on the GPU using two kernel launches:
//       1) Each block reduces its slice of the input into **one element**
//       2) A second launch (single block) reduces those partial sums
//   • Compare with the reference result computed on the host.
//
// Build:   nvcc -std=c++20 ex04_block_sum_student.cu -o ex04
// ─────────────────────────────────────────────────────────────────────────────
//
// Follow the naming conventions introduced in Exercise 03:
//   kBlockSize, kNumElements, numBlocks, threadsPerBlock, blocksPerGrid, …
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include <cuda_runtime.h>

#include "cuda_check.h"

// ---------------------------------------------------------------------------
// Configuration (you may tweak)
// ---------------------------------------------------------------------------
constexpr int  kDeviceId    = 0;       // change if you were assigned a different GPU
constexpr int  kBlockSize   = 256;     // must be a power‑of‑two for this reduction
constexpr int  kNumElements = 1 << 18; // 262 144 ints ≈ 1 MB

template <typename T>
constexpr std::size_t Bytes(std::size_t n) { return n * sizeof(T); }

// ---------------------------------------------------------------------------
// Kernel declarations (implement them in Part 4 & 5)
// ---------------------------------------------------------------------------
__global__ void blockReduceKernel(const int* __restrict__ d_input,
                                  int*  __restrict__ d_partial,
                                  int           numElements);

__global__ void finalReduceKernel(int* d_partial, int numPartials);

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
  // ───►►► Part 1 of 8 – choose device, create stream ◄◄◄─────────────────────
  CUDA_CHECK(cudaSetDevice(kDeviceId));
  cudaStream_t stream;
  CUDA_CHECK(cudaStreamCreate(&stream));

  // Host‑side random input
  std::mt19937                     rng{std::random_device{}()};
  std::uniform_int_distribution<>  dist(-10, 10);
  std::vector<int> h_input(kNumElements);
  for (auto& v : h_input) v = dist(rng);

  const int hostResult = std::accumulate(h_input.begin(), h_input.end(), 0);
  std::cerr << "Host sum: " << hostResult << '\n';

  // ───►►► Part 2 of 8 – copy input to device ◄◄◄────────────────────────────
  int* d_input = nullptr;
  CUDA_CHECK(cudaMallocAsync(&d_input, Bytes<int>(kNumElements), stream));
  CUDA_CHECK(cudaMemcpyAsync(d_input, h_input.data(), Bytes<int>(kNumElements),
                             cudaMemcpyHostToDevice, stream));

  // ───►►► Part 3 of 8 – allocate device buffer for partial sums ◄◄◄─────────
  // TODO: compute numBlocks and allocate d_partial accordingly.
  int  numBlocks  = /* TODO */;
  int* d_partial  = nullptr;
  // TODO: cudaMallocAsync for d_partial

  // ───►►► Part 4 of 8 – launch first reduction kernel ◄◄◄───────────────────
  // TODO: configure grid & block and launch blockReduceKernel
  // Example:
  //   blockReduceKernel<<<numBlocks, kBlockSize, 0, stream>>>(d_input,
  //                                                           d_partial,
  //                                                           kNumElements);
  // CUDA_CHECK(cudaGetLastError());

  // ───►►► Part 5 of 8 – launch second (final) kernel ◄◄◄────────────────────
  // TODO: launch finalReduceKernel with one block (kBlockSize threads)
  // CUDA_CHECK(cudaGetLastError());

  // ───►►► Part 6 of 8 – copy result back ◄◄◄────────────────────────────────
  int deviceResult = 0;
  // TODO: cudaMemcpyAsync to copy the result back

  // ───►►► Part 7 of 8 – cleanup device allocations ◄◄◄─────────────────────
  // TODO: cudaFreeAsync for d_input and d_partial
  // TODO: synchronize and destroy stream

  // ───►►► Part 8 of 8 – verify ◄◄◄──────────────────────────────────────────
  std::cout << "Device sum: " << deviceResult << '\n';
  // TODO: compare deviceResult with hostResult (assert or if‑statement)

  return 0;
}

// ---------------------------------------------------------------------------
// Kernel definitions (Part 4 & 5) – leave blank for students
// ---------------------------------------------------------------------------
__global__ void blockReduceKernel(const int* __restrict__ d_input,
                                  int*  __restrict__ d_partial,
                                  int           numElements)
{
  // TODO: allocate shared memory (kBlockSize ints) via __shared__
  // TODO: load element (or 0 if out‑of‑range) into shared memory
  // TODO: perform reduction within the block (shared‑memory or warp shuffles)
  // TODO: write block sum to d_partial[blockIdx.x]
}

__global__ void finalReduceKernel(int* d_partial, int numPartials)
{
  // TODO: final reduction of 'numPartials' values into d_partial[0]
}
