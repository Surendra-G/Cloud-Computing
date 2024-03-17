#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int parcel = 1; // Initial value of the parcel

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        printf("Error: Minimum 3 processes required.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        MPI_Send(&parcel, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&parcel, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Parcel value at rank 0: %d\n", parcel);
    } else {
        MPI_Recv(&parcel, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        parcel += 1;
        if (rank < size - 1) {
            MPI_Send(&parcel, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(&parcel, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
