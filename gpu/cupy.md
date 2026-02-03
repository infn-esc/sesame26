---
title: Introduction to GPU Programming with CuPy
layout: main
section: gpu
---

The goal of this section is to introduce you to GPU programming using CuPy.
## set up

`ssh` to the machine with this command:
```bash
ssh -L XXXX:localhost:XXXX <username>@<machine-address>
```
where XXXX is a 4-digit number, unique for each user.

Then setup the Python enviromenti with:
```bash
source /usr/share/esc-env/bin/activate
```

Now you can start JupyterLab with the command:
```bash
jupyter-lab --port XXXX
```
Once JupyterLab is running, you will find in the output a URL like this:

```bash
...
    Or copy and paste one of these URLs:
        http://localhost:9004/lab?token=8928e7071...
        http://127.0.0.1:9004/lab?token=8928e7071...
...
```
Paste one of these URLs in your local browser, and you should see the JupyterLab page.

## Exercises

### Exercise 1: port NumPy code to CuPy

Take the code written with NumPy and modify it to use CuPy. Time both executions to compare CPU vs. GPU speedup.

**Note:** GPU operations are asynchronous. Use:

```python
cp.cuda.Device().synchronize()
```

before stopping your timer to get accurate timings.

You can try the same with the Python code of your personal project!

### Exercise 2: transfers and GPU allocation

1. Create two arrays of `N` random numbers on CPU
2. Copy them to the GPU
3. Sum them
4. Copy them back to the CPU.

Now avoid the copy to the GPU by creating the random arrays directly on the device with CuPy.

### Exercise 3: write a kernel

Write a kernel (take one from the CUDA exercises or write your own) with CuPy using:

* `cp.RawKernel`
* `cp.ElementwiseKernel` (you can use the variable `i` for the the index within the loop and method `_ind.size()` for the total number of elements to apply the elementwise operation)

### Exercise 4: reduction

Implement the reduction kernel:

* using `cp.RawKernel` and the CUDA kernel you wrote during the CUDA part
* using `cp.ReductionKernel`
* using `cp.sum`

Tips:

* Add `%%time` or `time.time()` + `cp.cuda.Device().synchronize()` for accurate timing.
* Check GPU utilization using `watch nvidia-smi` while kernels run.
* Experiment with different vector sizes to check GPU utilization and speed-up.
