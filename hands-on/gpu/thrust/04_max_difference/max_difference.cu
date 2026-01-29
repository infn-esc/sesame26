
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

int main() {
	auto h_v1 = thrust::host_vector<int>{1, 7, 2, 8, -1, 0};
	auto h_v2 = thrust::host_vector<int>{5, 2, 6, -3, 9, 4};

	thrust::device_vector<int> d_v1 = h_v1;
	thrust::device_vector<int> d_v2 = h_v2;

	// Part 1 of 1: Compute the maximum difference between the two vectors
	// Hint: use transform and zip iterators

	std::cout << "The max difference is " << max_difference << std::endl;
}
