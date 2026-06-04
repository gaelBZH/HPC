#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000 // Large size for time measurement
#define M 256      // Threads per block

__global__ void add_gpu(int *a, int *b, int *c, int n)
{
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if(index < n)
        c[index] = a[index] + b[index];
}

void add_cpu(int *a, int *b, int *c, int n)
{
    for(int i = 0; i < n; i++)
        c[i] = a[i] + b[i];
}

void random_fill(int *tab, int size)
{
    for(int i = 0; i < size; i++)
        tab[i] = rand() % 101;
}

int main(void)
{
    int *a;
	int *b;
	int *c_cpu;
	int *c_gpu;
    int *d_a;
	int *d_b;
	int *d_c;
    size_t size = N * sizeof(int);
    srand(time(NULL));

    // Allocate host memory
    a = (int *)malloc(size);
    b = (int *)malloc(size);
    c_cpu = (int *)malloc(size);
    c_gpu = (int *)malloc(size);
    random_fill(a, N);
    random_fill(b, N);

    // CPU Calculation
    clock_t cpu_start = clock();
    add_cpu(a, b, c_cpu, N);
    clock_t cpu_end = clock();
    double cpu_time = ((double)(cpu_end - cpu_start)) / CLOCKS_PER_SEC;

    // GPU Preparation
    cudaEvent_t start_total, stop_total, start_compute, stop_compute;
    cudaEventCreate(&start_total); cudaEventCreate(&stop_total);
    cudaEventCreate(&start_compute); cudaEventCreate(&stop_compute);

    // Total GPU Time
    cudaEventRecord(start_total);

    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    int blocks = (N + M - 1) / M;

    // GPU Compute Only
    cudaEventRecord(start_compute);
    add_gpu<<<blocks, M>>>(d_a, d_b, d_c, N);
    cudaEventRecord(stop_compute);
    cudaEventSynchronize(stop_compute);

    cudaMemcpy(c_gpu, d_c, size, cudaMemcpyDeviceToHost);

    cudaEventRecord(stop_total);
    cudaEventSynchronize(stop_total);

    // Calculate GPU times
    float gpu_compute_time = 0;
	float gpu_total_time = 0;
    cudaEventElapsedTime(&gpu_compute_time, start_compute, stop_compute);
    cudaEventElapsedTime(&gpu_total_time, start_total, stop_total);

    // Print Results
    printf("Vector size: %d\n", N);
    printf("1. CPU Time: %f ms\n", cpu_time * 1000.0);
    printf("2. GPU Compute Only Time: %f ms\n", gpu_compute_time);
    printf("3. GPU Total Time (with Memory): %f ms\n", gpu_total_time);

    // Free Memory
    free(a);
	free(b);
	free(c_cpu);
	free(c_gpu);
    cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

    return 0;
}