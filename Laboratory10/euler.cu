#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100000000 // Number of iterations
#define THREADS_PER_BLOCK 256

// Parallel execution using CUDA
__global__ void euler_gpu(int n, double *global_sum)
{
    int index = threadIdx.x + blockIdx.x * blockDim.x + 1; // Start from k=1
    int stride = blockDim.x * gridDim.x;
    double local_sum = 0.0;

    // Grid-stride loop to handle arbitrary N
    for (int k = index; k <= n; k += stride)
        local_sum += 1.0 / (double)k;

    // Atomic addition to safely combine results from all threads
    atomicAdd(global_sum, local_sum);
}

// Sequential execution on CPU
double euler_cpu(int n)
{
    double sum = 0.0;
    for (int k = 1; k <= n; k++)
        sum += 1.0 / (double)k;
    return sum - log((double)n);
}

int main()
{
    double *d_sum;
    double h_sum = 0.0;
    double gamma_gpu;
    double gamma_cpu;

    // CPU Timing and Execution
    clock_t cpu_start = clock();
    gamma_cpu = euler_cpu(N);
    clock_t cpu_end = clock();
    double cpu_time = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    // GPU Timing and Execution
    cudaMalloc((void **)&d_sum, sizeof(double));
    cudaMemcpy(d_sum, &h_sum, sizeof(double), cudaMemcpyHostToDevice);

    int blocks = (N + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);

    cudaEventRecord(start);
    euler_gpu<<<blocks, THREADS_PER_BLOCK>>>(N, d_sum);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    cudaMemcpy(&h_sum, d_sum, sizeof(double), cudaMemcpyDeviceToHost);
    gamma_gpu = h_sum - log((double)N); // end : Subtract ln(n)

    float gpu_time = 0;
    cudaEventElapsedTime(&gpu_time, start, stop);

    // Check and Compare
    printf("Iterations (N): %d\n\n", N);

    printf("CPU Result (Gamma) : %.10f\n", gamma_cpu);
    printf("GPU Result (Gamma) : %.10f\n", gamma_gpu);
    printf("Difference         : %.10f\n\n", fabs(gamma_cpu - gamma_gpu));
    printf("CPU Execution Time : %f ms\n", cpu_time * 1000.0);
    printf("GPU Execution Time : %f ms\n", gpu_time);

    cudaFree(d_sum);
    return 0;
}