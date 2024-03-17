#include <stdio.h>
#include <mpi.h>

int is_prime(int n) {
    if (n <= 1)
        return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    int rank, size;
    int nstart = 1, nfinish = 10000;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("%s : Prime numbers between %d and %d are :\n", argv[0], nstart, nfinish);
    }

    int chunk_size = (nfinish - nstart + 1) / size;
    int start = nstart + rank * chunk_size;
    int end = start + chunk_size - 1;
    if (rank == size - 1) {
        end = nfinish;
    }

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            printf("%s : %d\n", argv[0], i);
        }
    }

    MPI_Finalize();
    return 0;
}
