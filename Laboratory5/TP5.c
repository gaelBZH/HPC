#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sizes[] = {500, 1000, 2000, 3000, 4000};
#define NUM_TESTS 5
#define SPARSITY 0.1

void run_test(int N, int rank, int size)
{
    // CRS Matrix
    double *val = NULL;
    int *col_ind = NULL;
    int *row_ptr = NULL;
    
    // Calculate Remainder
    int rows_per_proc = N / size;
    int remainder = N % size;
    int local_rows = (rank < remainder) ? rows_per_proc + 1 : rows_per_proc;
    int start_row = (rank < remainder) ? rank * (rows_per_proc + 1) : rank * rows_per_proc + remainder;

    double *full_vector = malloc(N * sizeof(double));
    double *local_res = malloc(local_rows * sizeof(double));
    double *final_res = NULL;
    int total_notNullValues = 0;
    double t1;
    double t2;



    // Create CRS Matrix
    if (rank == 0)
    {
        final_res = malloc(N * sizeof(double));
        row_ptr = malloc((N + 1) * sizeof(int));
        row_ptr[0] = 0;
        val = malloc(N * N * SPARSITY * 2 * sizeof(double)); 
        col_ind = malloc(N * N * SPARSITY * 2 * sizeof(int));

        for (int i = 0; i < N; i++)
        {
            int row_notNullValues = 0;
            for (int j = 0; j < N; j++)
            {
                if ((double)rand() / RAND_MAX < SPARSITY)
                {
                    val[total_notNullValues] = (double)rand() / RAND_MAX;
                    col_ind[total_notNullValues] = j;
                    total_notNullValues += 1;
                    row_notNullValues += 1;
                }
            }
            row_ptr[i+1] = row_ptr[i] + row_notNullValues;
            full_vector[i] = (double)rand() / RAND_MAX;
        }
        t1 = MPI_Wtime();
    }



    // Broadcast
    MPI_Bcast(full_vector, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int notNullValues;
    int *send_counts = malloc(size * sizeof(int));
    int *displs = malloc(size * sizeof(int));
    int *recv_counts = malloc(size * sizeof(int));
    int *recv_displs = malloc(size * sizeof(int));

    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
        {
            int i_rows = (i < remainder) ? rows_per_proc + 1 : rows_per_proc;
            int i_start = (i < remainder) ? i * (rows_per_proc + 1) : i * rows_per_proc + remainder;
            
            send_counts[i] = row_ptr[i_start + i_rows] - row_ptr[i_start];
            displs[i] = row_ptr[i_start];
            
            recv_counts[i] = i_rows;
            recv_displs[i] = i_start;
        }
    }

    MPI_Scatter(send_counts, 1, MPI_INT, &notNullValues, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double *local_val = malloc(notNullValues * sizeof(double));
    int *local_col_ind = malloc(notNullValues * sizeof(int));
    int *local_row_ptr = malloc((local_rows + 1) * sizeof(int));

    MPI_Scatterv(val, send_counts, displs, MPI_DOUBLE, local_val, notNullValues, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(col_ind, send_counts, displs, MPI_INT, local_col_ind, notNullValues, MPI_INT, 0, MPI_COMM_WORLD);




    // Send the row_ptr parts
    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            int i_rows = recv_counts[i];
            int i_start = recv_displs[i];
            int offset = row_ptr[i_start];
            int *temp_ptr = malloc((i_rows + 1) * sizeof(int));
            for (int j = 0; j <= i_rows; j++) 
                temp_ptr[j] = row_ptr[i_start + j] - offset;
            MPI_Send(temp_ptr, i_rows + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            free(temp_ptr);
        }
        for (int j = 0; j <= local_rows; j++)
            local_row_ptr[j] = row_ptr[j];
    }
    else
        MPI_Recv(local_row_ptr, local_rows + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);



    // Calculate
    for (int i = 0; i < local_rows; i++)
    {
        local_res[i] = 0.0;
        for (int k = local_row_ptr[i]; k < local_row_ptr[i + 1]; k++)
            local_res[i] += local_val[k] * full_vector[local_col_ind[k]];
    }



    // Get and Merge Results
    MPI_Gatherv(local_res, local_rows, MPI_DOUBLE, final_res, recv_counts, recv_displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);



    // Print Results
    if (rank == 0)
    {
        t2 = MPI_Wtime();
        printf("Matrix Size : %dx%d\t| Time : %f s\t", N, N, t2 - t1);

        // Verification
        int correct = 1; // boolean
        for (int i = 0; i < N; i++)
        {
            double seq_val = 0;
            for (int k = row_ptr[i]; k < row_ptr[i+1]; k++)
                seq_val += val[k] * full_vector[col_ind[k]];
            
            if (abs(seq_val - final_res[i]) > 1e-7)
                correct = 0;
        }
        if (correct)
            printf("Verification Successfull\n");
        else
            printf("Verification Failed\n");

        // Free Matrix
        free(val);
        free(col_ind);
        free(row_ptr);
        free(final_res);
    }




    // Free Memory
    free(full_vector);
    free(local_res);
    free(local_val); 
    free(local_col_ind);
    free(local_row_ptr);
    free(send_counts);
    free(displs);
    free(recv_counts);
    free(recv_displs);
}




int main(int argc, char** argv)
{
    int rank; // process ID
    int size; // number of processes
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        srand(time(NULL));
        printf("=========== Test with %d processes ===========\n", size);
    }

    // Constant Number of Processes
    for (int i = 0; i < NUM_TESTS; i++)
        run_test(sizes[i], rank, size);

    MPI_Finalize();
    return 0;
}

// mpicc TP5.c -o emp && mpirun -np 8 ./emp