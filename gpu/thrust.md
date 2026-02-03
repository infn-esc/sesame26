---
title: Introduction to Thrust
layout: main
section: gpu
---

The goal of this section is to introduce you to GPU programming using Thrust.
# Thrust

The four labs below mirror the CUDA ones but use the Thrust C++ library. All code lives in hands-on/gpu/thrust/XX_<name>/.
Every exercise follows the same workflow:

1. Open the `.cu` file and replace each ►►► TODO ◄◄◄.
2. Build with the provided Makefile:   `$ make`   
3. Run → the program prints “…PASSED 🎉” (or an assert succeeds).

### Exercise 1 – Memory Model with Thrust Vectors

*Folder*: thrust/01_memory_model
*Goal*: practice host ↔ device copies and Thrust containers.
Step	What you implement

1	Initialize a `thrust::host_vector<int> h_buf(dim)` with 0, 1, 2, ... (hint : `thrust::sequence`)
2	Create two `thrust::device_vector<int>` buffers `d1`, `d2`.
3	Copy host → d1 with any Thrust copy method.
4	Copy d1 → d2 using a different method (e.g. explicit iterator pair).
5	Zero h_buf on the CPU.
6	Copy d2 → h_buf and assert that values are again 0…dim-1.

Build & run:
```
cd hands-on/gpu/thrust/01_memory_model
make         # builds memory_model
./memory_model
```
Variation: time the H↔D copy using cudaEvent_t and compare to the raw‐pointer CUDA version.
### Exercise 2 – Your First Thrust “Kernel”

*Folder* : thrust/02_my_first_kernel

*Goal* : launch-free vector operations and async transfers.

    Pinned-host buffer h_a (use thrust::universal_vector or cudaMallocHost).

    Device buffer d_a (thrust::device_vector<int>(N)).

    Fill d_a with i + 42 using thrust::sequence.

    Create a cudaStream_t queue.

    Asynchronously copy d_a → h_a with thrust::copy(thrust::cuda::par.on(queue), …).

    Verify with an assert that h_a == {42, 43, …}.

Build:
```
cd hands-on/gpu/thrust/02_my_first_kernel
make
```
Run prints Correct, good work!.

Tip: wrap the Thrust copy in CUDA_CHECK(cudaStreamSynchronize(queue)); before destroying the stream to avoid race conditions.
### Exercise 3 – Data Statistics (Mean & Stdev)

*Folder *: thrust/03_data_statistics
*Goal *: combine transform and reduce to compute simple stats.

1.	Host vector `h_data(N)`; fill with random floats (provided helper).
2.	Copy to `thrust::device_vector<float> d_data`.
3.	`Mean = thrust::reduce(d_data.begin(), d_data.end()) / N`.
4.	`squared_diffs[i] = (d_data[i] – mean)²` via `thrust::transform` with a lambda or functor.
5.	`Stdev = sqrt(reduce(squared_diffs)/N)`.
6.	Challenge: recompute stdev without the intermediate buffer (use a binary transform-reduce).

Build & run:
```
cd hands-on/gpu/thrust/03_data_statistics
make test      # Makefile prints mean & σ and checks against CPU reference
```
Hints

```
    thrust::placeholders::_1 can shorten lambdas (_1 - mean).
```

    For the "no buffer" variant use `thrust::transform_reduce`.

### Exercise 4 – Maximum Absolute Difference (Zip Iterators)

*Folder*: thrust/04_max_difference
*Goal*: learn zip iterators and element-wise transforms.

Two small host vectors `h_v1`, `h_v2` already provided.

Copy to `d_v1`, `d_v2`.

Use `thrust::transform` with a zip iterator pair to compute
`|v1-v2|` into a temporary `device_vector<int>` diffs.

```
    auto first = thrust::make_zip_iterator(thrust::make_tuple(d_v1.begin(),
                                                              d_v2.begin()));
    auto last  = first + d_v1.size();
    thrust::transform(first, last, diffs.begin(),
                      [] __device__ (auto t) {
                        int a = thrust::get<0>(t);
                        int b = thrust::get<1>(t);
                        return abs(a - b);
                      });

    max_difference = thrust::reduce(diffs.begin(), diffs.end(), 0, thrust::maximum<int>());
```

    Print the result and compare to a CPU calculation.

Build:
```
cd hands-on/gpu/thrust/04_max_difference
make
```

### Common Pitfalls & Tips

    Prefer algorithms (transform, reduce) over raw loops. Thrust chooses good launches for you.

    Explicit streams: .on(my_stream) works with every algorithm.

    device_vector reallocation is costly—reserve capacity if you grow it.

    Thrust follows C++20 ranges concepts; use std::views on the host for quick sanity checks.

