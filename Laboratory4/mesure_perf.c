#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// mpicc mesure_perf.c -o emp && mpirun -np 2 ./emp

int main(int argc, char** argv)
{
    int rank;
    int size;
    int sizes[] = {100, 1000, 10000, 100000, 500000, 1000000}; // 6 amounts of integers sent
    double t1;
    double t2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2)
    {
        if (rank == 0)
        printf("Not enough processes ! (Got %i)\n", size);
        MPI_Finalize();
        return 0;
    }

    for (int i = 0; i < 6; i++)
    {
        int count = sizes[i];
        int* buffer = (int*)malloc(count * sizeof(int));

        if (rank == 0) { 
            t1 = MPI_Wtime();
            MPI_Send(buffer, count, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, count, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            t2 = MPI_Wtime();

            printf("Size: %d Bytes \t| Time: %f s\n", (int)(count * sizeof(int)), (t2 - t1) / 2);
        } 
        else if (rank == 1)
        {
            MPI_Recv(buffer, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, count, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        free(buffer);
    }

    MPI_Finalize();
    return 0;
}