#include <stdio.h>
#include <mpi.h>

#define NUMDATA 10000
int data[NUMDATA];
void LoadData(int data[]) {
    for (int i = 0; i < NUMDATA; i++) {
        data[i] = 1;
    }
}
int AddUp(int data[], int start, int end) {
    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += data[i];
    }
    return sum;
}
int main(int argc, char **argv) {
    int rank, size;
    int sum = 0, partial_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        LoadData(data);
    }
    MPI_Bcast(data, NUMDATA, MPI_INT, 0, MPI_COMM_WORLD);
    int chunk_size = NUMDATA / size;
    int start = rank * chunk_size;
    int end = (rank == size - 1) ? NUMDATA : start + chunk_size;
    partial_sum = AddUp(data, start, end);
    MPI_Reduce(&partial_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("The total sum of data is %d\n", sum);
    }

    MPI_Finalize();
    return 0;
}
