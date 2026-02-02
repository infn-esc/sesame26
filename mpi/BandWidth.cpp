#include "mpi.h"
#include <iostream>

//Do not use this code for real  BandWitdh tests. You can find professional test here:
// https://www.intel.com/content/www/us/en/docs/mpi-library/user-guide-benchmarks/2021-2/overview.html

int main(int argc, char* argv[])
{
  int numranks, rank, dest, source, rc, count, tag = 1;
  double       t1, t2, tmin, total_size, bw;
  int          i, j, k, nloop;

// define here the size of the array that will be created
//  int msg_size = 10;
//  int msg_size = 10000;

  int msg_size = 40000000; //test bw with nloop = 10

//////////////////////////////////////////////////////////////

  int *inmsg = new int[msg_size]();
  if (inmsg == nullptr) {
     std::cout << "Error: memory could not be allocated";
     MPI_Abort( MPI_COMM_WORLD, 1 );
  }

  int *outmsg = new int[msg_size]();
  if (outmsg == nullptr) {
     std::cout << "Error: memory could not be allocated";
     MPI_Abort( MPI_COMM_WORLD, 1 );
  }

  std::cout << "Arrays created and initialized" << std::endl;


// Define here the number of Ping-Pong Iterations
  nloop = 1;
  nloop = 20;

//////////////////////////////////////////////////////////


  MPI_Status Stat; // required variable for receive routines
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// rank 0 sends to rank 1 and waits to receive a return message
//
// Start the PingPong here.....
  if (rank == 0) {
     dest = 1;
     source = 1;
     t1 = MPI_Wtime();
     for (j=0; j<nloop; j++) {
        MPI_Ssend(outmsg, msg_size, MPI_INT, dest, tag, MPI_COMM_WORLD);
        MPI_Recv(inmsg, msg_size, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
     }
     t2 = (MPI_Wtime() - t1);
  }
// rank 1 waits for rank 0 message then returns a message
  else if (rank == 1) {
     std::cout << "Rank 1 is waiting for a message from Rank 0" << std::endl;
     source = 0;
     dest = 0;
     for (j=0; j<nloop; j++) {
        MPI_Recv(inmsg, msg_size, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
        MPI_Ssend(outmsg, msg_size, MPI_INT, dest, tag, MPI_COMM_WORLD);
     }
  }

// Query receive Stat variable and print message details
  MPI_Get_count(&Stat, MPI_INT, &count);
  std::cout <<"Rank " << rank << " Received " << count << " INTs from  rank " << Stat.MPI_SOURCE << " with tag " << Stat.MPI_TAG << std::endl;

  if (rank == 0) {
//     double rate;
//          if (tmin > 0) rate = n * sizeof(double) * 1.0e-6 /tmin;
//          else          rate = 0.0;
     std::cout << nloop << " Iterations took " << t2 << " seconds" << std::endl;
     total_size = msg_size * sizeof(int) * nloop;
     bw = static_cast<double>(total_size) / t2;
     std::cout << total_size << " Bytes sent in " << t2 << " seconds" << std::endl << "Bandwidth = " << bw << " B/s = " << bw * 8 * 1e-9 << " Gbit/s" << std::endl;
  }

  delete[] inmsg;
  delete[] outmsg;

  MPI_Finalize();

}
