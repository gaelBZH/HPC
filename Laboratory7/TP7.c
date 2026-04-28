#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>
#include "tmeas.h"

#define N 1000000       // 1 million rows/colomns
#define K 1000          // 1000 non zero elements per row
#define TOTAL_NONZERO (long)N * K

int main()
{
    srand(time(NULL));

    printf("Memory Allocation (Total Non-Zero : %ld)...\n", TOTAL_NONZERO);
    printf("Please wait, this may take few seconds.\n");

    // Memory Allocation
    double *val = malloc(TOTAL_NONZERO * sizeof(double));
    int *col_ind = malloc(TOTAL_NONZERO * sizeof(int));
    int *row_ptr = malloc((N + 1) * sizeof(int));
    double *T = malloc(N * sizeof(double));
    double *crs_result = calloc(N, sizeof(double));

    if (!val || !col_ind || !row_ptr || !T || !crs_result)
        errx(EXIT_FAILURE, "Allocation Error.");



    // Generate CRS Data 
    printf("CRS Data Generation...\n");
    for (long i = 0; i < TOTAL_NONZERO; i++)
    {
        val[i] = (double)rand() / RAND_MAX;
        col_ind[i] = rand() % N;
    }
    for (int i = 0; i <= N; i++)
        row_ptr[i] = i * K;
    for (int i = 0; i < N; i++)
        T[i] = (double)rand() / RAND_MAX;




    // Time Mesures
    printf("Multiplication...\n");

    tstart();
    for (int i = 0; i < N; i++)
        for (int k = row_ptr[i]; k < row_ptr[i+1]; k++)
            crs_result[i] += val[k] * T[col_ind[k]];
    double duration = tstop();

    printf("\nResults\n");
    printf("Time : %f seconds\n", duration);

    // Performance and Bandwidth Calculations
    double flops = 2.0 * TOTAL_NONZERO; // 1 mul + 1 add per non-zero element
    double gflops = (flops / duration) / 1e9;
    
    double bytes = (double)TOTAL_NONZERO * (sizeof(double) + sizeof(int)) + (double)N * sizeof(double);
    double bandwidth = (bytes / duration) / 1e6; // MB/s

    printf("Performance : %.4f GFLOPS\n", gflops);
    printf("Bandwidth : %.2f MB/s\n", bandwidth);





    // Free Memory
    free(val);
    free(col_ind);
    free(row_ptr);
    free(T);
    free(crs_result);
    return 0;
}

/*
Processeur (CPU) : 12th Gen Intel(R) Core(TM) i7-12650H.

Vitesse de base : 2,30 GHz.

Cœurs : 10 (avec 16 processeurs logiques).

Cache L3 : 24,0 Mo.

Mémoire vive (RAM) : 32,0 Go.

Vitesse : 3200 MT/s.

Configuration : 2 emplacements utilisés sur 2 (SODIMM).
*/