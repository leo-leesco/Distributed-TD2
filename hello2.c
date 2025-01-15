#include <unistd.h>
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	MPI_Init(NULL,NULL);

	MPI_Barrier(MPI_COMM_WORLD);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		int size;
		MPI_Comm_size(MPI_COMM_WORLD, &size);

		printf("Hello World with %d ready task(s)\n", size);
	}

	MPI_Finalize();
}
