// C++ standard headers
#include <cassert>
#include <iostream>
#include <ranges>
#include <thrust/copy.h>
#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/universal_vector.h>
#include <thrust/sequence.h>

// CUDA headers
#include <cuda_runtime.h>

// local headers
#include "cuda_check.h"

// Here you can set the device ID that was assigned to you
#define MYDEVICE 0

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  CUDA_CHECK(cudaSetDevice(MYDEVICE));

  // Your problem's size
  int N = 1024;

  // Part 1 of 6: Allocate buffer in pinned host memory
  auto h_a = ...;

  // Part 2 of 6: Allocate buffer in device memory
  auto d_a = ...;

  // Part 3 of 6: Set every element in the device buffer to i + 42 (where i is the position in the buffer)
  // Hint: check the thrust::sequence algorithm

  // Part 4 of 6: Create a cuda stream

  // Part 5 of 6: Copy data from device to host asynchronously
  thrust::copy(thrust::cuda::par.on(queue), ...);

  // Part 6 of 6: Verify that the data returned to the host is correct
  assert(std::ranges::equal(h_a, std::views::iota(42) | std::views::take(N)));

  // Destroy the CUDA stream
  CUDA_CHECK(cudaStreamDestroy(queue));

  // If the program makes it this far, then the results are correct and
  // there are no run-time errors.  Good work!
  std::cout << "Correct, good work!" << std::endl;

}
