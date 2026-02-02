---
title: Introduction to MPI Programming 
layout: main
section: mpi
---

The goal of this section is to introduce you to MPI programming using OpenMPI.

### Hands-on

Remember to setup the environment as described in the Environment section:

1) ssh passwordless

2) shared working directory (you have to do nothing, is already there)

3) setup the correct export for PATH and LD_LIBRARY_PATH in the .bashrc

Check that your environment is correctly configured to compile MPI code by running:


```bash
$ mpic++ --version
g++ (GCC) 11.5.0 20240719 (Red Hat 11.5.0-5)
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Compile and run the MPI executable:
```bash
$ cd esc25/hands-on/mpi
$ mpic++ -o MPI_Hello.out MPI_Hello.cpp
$ mpirun  -n 2 MPI_Hello.out
$ mpirun -hostfile machinefile.txt -n 2 MPI_Hello.out
```

You can now go and try with the other exercises avalble on the esc25/hands-on/mpi directory (see the slides for more details):

1) The MPI PingPong

2) The Non Blocking MPI PingPong

3) The MPI Pi Computation

4) The Trivial MPI N_Body Simulation
