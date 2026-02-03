---
title: Introduction to MPI Programming 
layout: main
section: mpi
---

The goal of this section is to introduce you to MPI programming using OpenMPI.

## Hands-on

Remember to setup the environment as described in the Environment section:

1. ssh passwordless
1. shared working directory (you have to do nothing, is already there)
1. setup the correct export for `PATH` and `LD_LIBRARY_PATH` in the `.bashrc`

Check that your environment is correctly configured to compile MPI code by running:

```bash
$ mpic++ --version
g++ (GCC) 11.5.0 20240719 (Red Hat 11.5.0-11)
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Compile and run the MPI executable:

```bash
$ cd sesame26/hands-on/mpi
$ mpic++ -o MPI_Hello.out MPI_Hello.cpp
$ mpirun  -n 2 MPI_Hello.out
$ mpirun -hostfile machinefile.txt -n 2 MPI_Hello.out
```
You can now go and try with the other exercises avalble on the `sesame/hands-on/mpi` directory (see the slides for more details):

1. The MPI PingPong

```bash
$ mpic++ -o PingPong.out PingPong.cpp
$ mpirun -n 2 -hostfile machinefile.txt PingPong.out
```
2. The Non-Blocking MPI PingPong

```bash
$ mpic++ -o NoBloc_PingPong.out NoBloc_PingPong.cpp
$ mpirun -n 2 -hostfile machinefile.txt NoBloc_PingPong.out
```
3. The trivial BandWidth measure
```bash
$ mpic++ -o BandWidth.out BandWidth.cpp
$ mpirun -n 2 -hostfile machinefile.txt BandWidth.out  # this will use infiniband
$ mpirun -n 2 -hostfile machinefile.txt --mca btl_tcp_if_include eth0 BandWidth.out    #this will use ethernet
```
4. The MPI Pi Computation
```bash
$ mpic++ -o MPI_Pi.out MPI_Pi.cpp
$ mpirun -n 2 MPI_Pi.out    # try to increase the number of processes
```
5. The Trivial MPI N-Body Simulation

```bash
$ mpic++ -o MPI_Nbody.out MPI_Nbody.cpp
$ mpirun -n 2 MPI_Nbody.out    # try to increase the number of processes
```
