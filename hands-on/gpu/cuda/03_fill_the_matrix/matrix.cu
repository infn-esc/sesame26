// ─────────────────────────────────────────────────────────────────────────────
// Exercise 03 – Two‑Dimensional Grid & Block  (STUDENT VERSION)
//
// Goal:
//   • Launch a 2‑D grid of 2‑D blocks so each thread writes
//       M[row, col] = row * numCols + col
//   • Copy the matrix back to the host and verify the result.
//
// Build:   nvcc -std=c++20 ex03_fill_matrix.cu -o ex03
// ─────────────────────────────────────────────────────────────────────────────
//
// Recommended naming conventions (used consistently across all exercises):
//   • numRows   – number of rows  (Y dimension)
//   • numCols   – number of columns (X dimension)
//   • threadsPerBlock (dim3)  – geometry of a block
//   • blocksPerGrid  (dim3)   – geometry of the grid
//
// About dim3:
//   dim3 is a simple CUDA built‑in POD type that stores three
//   unsigned integers {x, y, z}. CUDA uses it to describe the size
//   of a grid or block:             <<<blocksPerGrid, threadsPerBlock>>>
//   Any component you omit defaults to 1, so `dim3 t(32, 8)` means
//   {x=32, y=8, z=1}.  Internally it is defined as
//        struct dim3 { unsigned int x, y, z; };
//   Nothing magical – just a convenient 3‑tuple.
//

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
constexpr int kDeviceId = 0;   // Change if you were assigned a different GPU

// ---------------------------------------------------------------------------
// Kernel
// ---------------------------------------------------------------------------
__global__ void fillMatrixKernel(int* __restrict__ mat,
                                 int   numCols,
                                 int   numRows)
{
  // 1. Identify my (row, col) inside the 2‑D matrix
  const int col = blockIdx.x * blockDim.x + threadIdx.x;
  const int row = blockIdx.y * blockDim.y + threadIdx.y;

}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{
  CUDA_CHECK(cudaSetDevice(kDeviceId));
  cudaStream_t stream;
  CUDA_CHECK(cudaStreamCreate(&stream));

  // ───►►► Part 1 of 4 – set matrix dims ◄◄◄─────────────────────────────────
  // Choose any positive sizes (e.g. 4×4 or 19×67) so long as both ≥ 1.
  const int numCols = /* TODO: set X dimension */;  // aka width
  const int numRows = /* TODO: set Y dimension */;  // aka height

  // Host allocation
  const int elements = numCols * numRows;
  std::vector<int> h_mat(elements, 0);
  const std::size_t bytes = elements * sizeof(int);

  // Device allocation
  int* d_mat = nullptr;
  // API: cudaMallocAsync(void** ptr, size_t size, cudaStream_t s)
  CUDA_CHECK(cudaMallocAsync(reinterpret_cast<void**>(&d_mat), bytes, stream));

  // ───►►► Part 2 of 4 – configure grid & block, launch kernel ◄◄◄──────────
  const dim3 threadsPerBlock(16, 16);  // fine for small matrices
  const dim3 blocksPerGrid(
      (numCols + threadsPerBlock.x - 1) / threadsPerBlock.x,
      (numRows + threadsPerBlock.y - 1) / threadsPerBlock.y);

  // Kernel launch
  fillMatrixKernel<<<blocksPerGrid, threadsPerBlock, 0, stream>>>(d_mat,
                                                                  numCols,
                                                                  numRows);
  CUDA_CHECK(cudaGetLastError());  // check launch

  // Device → host copy
  CUDA_CHECK(cudaMemcpyAsync(h_mat.data(), d_mat, bytes,
                             cudaMemcpyDeviceToHost, stream));

  // Free device memory (async)
  CUDA_CHECK(cudaFreeAsync(d_mat, stream));

  // Wait for completion
  CUDA_CHECK(cudaStreamSynchronize(stream));

  // ───►►► Part 3 of 4 – verify result ◄◄◄───────────────────────────────────
  for (int row = 0; row < numRows; ++row)
    for (int col = 0; col < numCols; ++col)
      assert(h_mat[row * numCols + col] == row * numCols + col);

  // Clean up
  CUDA_CHECK(cudaStreamDestroy(stream));
  std::cout << "Exercise 03 – 2‑D grid/block: PASSED 🎉" << std::endl;
  return 0;
}
