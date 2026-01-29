// ─────────────────────────────────────────────────────────────────────────────
// Exercise 2 – Launch Your First Kernel  (STUDENT VERSION)
//
// Goal:
//   • Allocate a device array d_a of N ints
//   • Launch a 1-D grid/block configuration so each GPU thread writes
//       d_a[i] = i + 42
//   • Copy the result back to the host and verify it
//
// Build:   nvcc -std=c++17 launch_kernel.cu -o ex02
// ─────────────────────────────────────────────────────────────────────────────

// C++ headers
#include <cassert>
#include <iostream>
#include <vector>

// CUDA headers
#include <cuda_runtime.h>

// Local helper
#include "cuda_check.h"

// ---------------------------------------------------------------------------
// Configuration
// ---------------------------------------------------------------------------
constexpr int kDeviceId       = 0;    // Change if you were assigned a different GPU
constexpr int kNumElements    = 64;   // Must be divisible by kBlockSize
constexpr int kBlockSize      = 8;    // Threads per block

// ---------------------------------------------------------------------------
// Kernel – each thread sets d_data[idx] = idx + 42
// ---------------------------------------------------------------------------
__global__ void initArrayKernel(/* TODO: add parameters */)
{
  // TODO: compute global thread index
  // TODO: guard against out-of-range accesses
  // TODO: write the value to global memory
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
  // Select device and create a stream
  CUDA_CHECK(cudaSetDevice(kDeviceId));
  cudaStream_t stream;
  CUDA_CHECK(cudaStreamCreate(&stream));

  // Host buffer (initially zeros)
  std::vector<int> h_a(kNumElements, 0);

  // Device buffer
  int* d_a = nullptr;
  const std::size_t bytes = kNumElements * sizeof(int);

  // ───►►► Part 1 of 5 – allocate device memory ◄◄◄──────────────────────────
  // API reference: cudaMallocAsync(void** ptr, size_t size, cudaStream_t s)
  // TODO: allocate d_a

  // ───►►► Part 2 of 5 – configure & launch kernel ◄◄◄──────────────────────
  const int numBlocks = kNumElements / kBlockSize;
  // API reference: <<<gridDim, blockDim, sharedMemBytes, cudaStream_t>>>
  // TODO: launch initArrayKernel

  // Optional: check launch errors (cudaGetLastError)

  // ───►►► Part 3 of 5 – copy device → host ◄◄◄─────────────────────────────
  // API reference: cudaMemcpyAsync(dst, src, bytes, cudaMemcpyKind, stream)
  // TODO: copy from d_a to h_a

  // ───►►► Part 4 of 5 – free device memory ◄◄◄─────────────────────────────
  // API reference: cudaFreeAsync(void* ptr, cudaStream_t s)
  // TODO: free d_a

  // Wait for completion
  CUDA_CHECK(cudaStreamSynchronize(stream));

  // Verify result
  for (int i = 0; i < kNumElements; ++i) { assert(h_a[i] == i + 42); }

  CUDA_CHECK(cudaStreamDestroy(stream));
  std::cout << "Exercise 2 – kernel launch: PASSED 🎉" << std::endl;
  return 0;
}
