
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

int main() {
  thrust::device_vector<int> d_v1{5, 2, 1, 9, 3, -1, 8, 4, 0, 6, 2, 3};

  // Part 1 of 2: Compute the 1D stencil of the device vector using radius = 3

  // Part 2 of 2: Copy the result back to the host vector and print it to check
  // that it's correct

  // BONUS: write a function that computes the stencil with a generic radius
}
