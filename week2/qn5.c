#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

int main(int argc, char **argv) {
    int rank, size;
    int letter_counts[ALPHABET_SIZE] = {0};
    char *file_content = NULL;
    long file_size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        FILE *file = fopen("WarAndPeace.txt", "r");
        if (file == NULL) {
            fprintf(stderr, "Error: Could not open file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Get file size
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Read file content into memory
        file_content = (char *)malloc((file_size + 1) * sizeof(char));
        fread(file_content, sizeof(char), file_size, file);
        fclose(file);

        // Null terminate the file content
        file_content[file_size] = '\0';

        // Calculate chunk size for each process
        int chunk_size = file_size / size;
        int remainder = file_size % size;
        int offset = 0;

        // Distribute chunks to processes
        for (int i = 1; i < size; i++) {
            int local_chunk_size = chunk_size + (i <= remainder ? 1 : 0);
            MPI_Send(&local_chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&file_content[offset], local_chunk_size, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            offset += local_chunk_size;
        }
    } else {
        int local_chunk_size;
        MPI_Recv(&local_chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        char *local_content = (char *)malloc((local_chunk_size + 1) * sizeof(char));
        MPI_Recv(local_content, local_chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        local_content[local_chunk_size] = '\0';

        // Count letters in local content
        for (int i = 0; i < local_chunk_size; i++) {
            char c = local_content[i];
            if (isalpha(c)) {
                c = tolower(c);
                letter_counts[c - 'a']++;
            }
        }
        free(local_content);
    }

    // Collect and aggregate letter counts
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int counts_received[ALPHABET_SIZE];
            MPI_Recv(counts_received, ALPHABET_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                letter_counts[j] += counts_received[j];
            }
        }

        // Print results
        printf("Letter counts:\n");
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            printf("%c: %d\n", 'a' + i, letter_counts[i]);
        }
    } else {
        MPI_Send(letter_counts, ALPHABET_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        free(file_content);
    }

    MPI_Finalize();
    return 0;
}
