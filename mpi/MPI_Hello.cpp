#include <iostream>
#include <mpi.h>

int main(int argc, char** argv){
   int rank, world_size;

//Initialize MPI
   MPI_Init(&argc, &argv);

// Get the rank of the process
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);

//Get thenumber of processes in current
   MPI_Comm_size(MPI_COMM_WORLD,&world_size);

  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;

//Get the processor name
  MPI_Get_processor_name(processor_name, &name_len);

  std::cout << "Hello world from processor " << processor_name << " rank " << rank << " of " << world_size << std::endl;

//Finalize MPI
   MPI_Finalize();
   return 0;
}
