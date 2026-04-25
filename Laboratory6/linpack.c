#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// DP (Double) / SP (Single) 
#ifdef SP
    typedef float REAL;
#else
    typedef double REAL;
#endif

// ROLL/UNROLL 
void daxpy(int n, REAL da, REAL dx[], REAL dy[])
{
    if (n <= 0)
        return;

    #ifdef DUNROLL
        // UNROLL
        int m = n % 4;
        for (int i = 0; i < m; i++)
            dy[i] = dy[i] + da * dx[i];
        for (int i = m; i < n; i += 4)
        {
            dy[i] = dy[i] + da * dx[i];
            dy[i+1] = dy[i+1] + da * dx[i+1];
            dy[i+2] = dy[i+2] + da * dx[i+2];
            dy[i+3] = dy[i+3] + da * dx[i+3];
        }
    #else
        // ROLL
        for (int i = 0; i < n; i++)
            dy[i] = dy[i] + da * dx[i];
    #endif
}

int main(int argc, char *argv[])
{
    int N = (argc > 1) ? atoi(argv[1]) : 100;
    REAL *matrix = malloc(N * N * sizeof(REAL));
    REAL *b = malloc(N * sizeof(REAL));
    
    // Init
    for(int i=0; i<N*N; i++)
        matrix[i] = (REAL)rand()/RAND_MAX;
    
    // Clock
    clock_t start = clock();
    for (int j = 0; j < N; j++)
        daxpy(N-j, 2.0, &matrix[j], &matrix[j]);
    clock_t end = clock();

    // Results
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("N=%d | GFLOPS: %f\n", N, (((2.0/3.0) * N * N * N) / time) / 1e9);

    // Free
    free(matrix);
    free(b);

    return 0;
}

// Double Precision (UNROLL)
// gcc -O3 -DDP -DUNROLL linpack.c -o linpack_dp_unroll -lm

// Double Precision (ROLL)
// gcc -O3 -DDP -DROLL linpack.c -o linpack_dp_roll -lm

// Single Précision  (UNROLL)
// gcc -O3 -DSP -DUNROLL linpack.c -o linpack_sp_unroll -lm

// Single Précision (ROLL)
// gcc -O3 -DSP -DROLL linpack.c -o linpack_sp_roll -lm