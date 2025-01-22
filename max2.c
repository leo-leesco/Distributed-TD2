#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define array_threshold 1000000

int max(int *array, int len) {
  int m = array[0];
  for (int i = 0; i < len; i++) {
    if (m < array[i]) {
      m = array[i];
    }
  }
  return m;
}

int slice_size(int len, int start, int step) {
  return len % step == 0 ? len / step : len / step + 1;
}

/**
 * returns the size of the slice
 * array is repopulated up to size with the array[start:len:step]
 */
int slice(int *array, int len, int start, int step) {
  int size = start;
  for (int i = 0; size < len; size += step) {
    array[i] = array[size];
    i++;
  }
  return size;
}

int min(int a, int b) { return a < b ? a : b; }

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int N_machines;
  MPI_Comm_size(MPI_COMM_WORLD, &N_machines);

  if (argc < 3) {
    printf("Usage: %s seed A\n", argv[0]);
    printf("\tseed: seed for pseudo-random generator\n");
    printf("\tA: size of the array\n");
    exit(1);
  }

  int seed = atoi(argv[1]);
  int A = atoi(argv[2]);
  srand48(seed);

  int array[A];
  for (int i = 0; i < A; i++) {
    array[i] = lrand48() % A;
  }

  double t_init = MPI_Wtime();
  // number of processes actually used, in case you
  // ask for too many processes anyway, at some point,
  // there will never be a use for all processes to
  // compute in parallel at the same time, and it'll
  // be easier to simply work sequentially
  int n = min(N_machines, A / array_threshold + 1);
  int size, temp_max;
  while (A > array_threshold && rank < n) {
    size = slice(array, A, rank, n);
    temp_max = max(array, size);
    MPI_Scatter(&temp_max, 1, MPI_INT, array, n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gather(array, n, MPI_INT, &temp_max, 1, MPI_INT, 0, MPI_COMM_WORLD);
    A = n;
    n = min(N_machines, A / array_threshold + 1);
  }
  if (rank == 0) {
    printf("%d, time elapsed = %f", max(array, A), MPI_Wtime() - t_init);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
