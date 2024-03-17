#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int size;
    int rank;
    int tag = 0;
    int count;
    MPI_Status status;
    int *data = NULL; // Pointer to dynamically allocated memory
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < size - 1; i++) {
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);
            data = (int *)malloc(count * sizeof(int)); // Dynamically allocate memory based on the received count
            MPI_Recv(data, count, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
            printf("Node ID: %d; tag: %d; MPI_Get_count: %d; \n", status.MPI_SOURCE, status.MPI_TAG, count);
            free(data); // Free dynamically allocated memory after use
            data = NULL; // Set pointer to NULL to avoid dangling pointer
        }
    } else {
        int send_count = rand() % 100;
        data = (int *)malloc(send_count * sizeof(int)); // Dynamically allocate memory for sending
        MPI_Send(data, send_count, MPI_INT, 0, tag, MPI_COMM_WORLD);
        free(data); // Free dynamically allocated memory after sending
    }

    MPI_Finalize();
    return 0;
}
