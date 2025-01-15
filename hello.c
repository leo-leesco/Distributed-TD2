#include <unistd.h>
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	MPI_Init(NULL,NULL);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char name[64];
	gethostname(name, 64);

	printf("Hello World from task %d out of %d on %s\n", rank, size, name);

	MPI_Finalize();
}
