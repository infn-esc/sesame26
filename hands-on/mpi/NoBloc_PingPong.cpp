#include <iostream>
#include <mpi.h>


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int comm_size;
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	if(comm_size != 2)
	{
		std::cout << "This application must be run with 2 MPI processes." << std::endl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}

	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if(my_rank == 0)
	{
    
                // Rank 0 sends data to rank 1
                // -----------------------------------------
                // Non-blocking receive for the reply
		int value_received = 0;
		MPI_Request recv_request;
                MPI_Irecv(&value_received, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &recv_request);
		int value_sent = 9999;
		MPI_Request send_request;

		// Launch the non-blocking send
		MPI_Isend(&value_sent, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &send_request);
		std::cout << "MPI process " << my_rank << " I launched the non-blocking send." << std::endl;

		// Wait for the non-blocking send to complete
		MPI_Wait(&send_request, MPI_STATUS_IGNORE);
		std::cout << "MPI process " << my_rank << " The wait completed, so I sent value " << value_sent << std::endl;
		// Wait for the non-blocking recv to complete
		MPI_Wait(&recv_request,MPI_STATUS_IGNORE);
		std::cout << "MPI process " << my_rank << " The wait completed, so I receive value " << value_sent << std::endl;
	}
	else
	{
		int value_received = 0;
		MPI_Request recv_request;
		MPI_Request send_request;

		// Launch the non-blocking receive
		MPI_Irecv(&value_received, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_request);
		std::cout << "MPI process " << my_rank << " I launched the non-blocking receive." << std::endl;

		// Wait for the non-blocking send to complete
		MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
		std::cout << "MPI process " << my_rank << " The wait completed, so I received value " << value_received << std::endl;
		// Non-blocking send back to rank 0
		MPI_Isend(&value_received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &send_request);
		std::cout << "MPI process " << my_rank << " The wait completed, so I sent back value " << value_received << std::endl;
                // Wait for send
                MPI_Wait(&send_request, MPI_STATUS_IGNORE);

	}

	MPI_Finalize();

	return 0;
}
