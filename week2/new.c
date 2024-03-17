#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int size, rank;
    int ball = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 2) {
        printf("This program needs to run on exactly 2 processes\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        for (int i = 1; i <= 10; i++) {
            MPI_Send(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&ball, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d received ball value: %d\n", rank, ball);
            if (ball == 10) {
                break;
            }
        }
    } else {
        for (int i = 1; i <= 10; i++) {
            MPI_Recv(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            ball++;
            MPI_Send(&ball, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            printf("Process %d received ball value: %d\n", rank, ball);
        }
    }

    MPI_Finalize();
    return 0;
}
