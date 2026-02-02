#include "mpi.h"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[])
{

   constexpr long long int num_steps = 4e10;
   long long int steps_per_process = 0;
   double pi = 0., mypi =0. ;
   constexpr double step = 1.0/(double) num_steps;
   double sum = 0.;
   int rank, world_size, myid, num_procs;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&world_size);

   myid = rank; num_procs = world_size;
   steps_per_process = num_steps / num_procs;

   std::cout << "Integrating Pi with numsteps = " << num_steps << ". Step = " << step << "." << std::endl;
   std::cout << "Numsteps per process = " << steps_per_process << "." << std::endl;

   double start_x = (myid * 1.0 / num_procs);

   for (long long int i=0; i < steps_per_process; i++){
      auto x = start_x + (i + 0.5)*step;
      sum = sum + 4.0/(1.0 +x*x);
   }

   mypi = step * sum;

   MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   if (myid == 0) {
      std::cout << "result: " << std::setprecision(15) << pi << std::endl;
   }

   MPI_Finalize();
   return 0;
}
