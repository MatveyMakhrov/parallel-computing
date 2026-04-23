#include <cuda.h>
#include <cuda_runtime.h>
#include "../include/vec_ops.h"

// ЯДРА 

// Умножение
__global__ void VecMulKernel(float *a, float *b, float *c, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
        c[i] = a[i] * b[i];
}

// Сложение
__global__ void VecAddKernel(float *a, float *b, float *c, int n)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
        c[i] = a[i] + b[i];
}

// GPU ФУНКЦИИ 

void vec_mul_cuda(float *a, float *b, float *c, int n)
{
    int SizeInBytes = n * sizeof(float);

    float *a_gpu = nullptr;
    float *b_gpu = nullptr;
    float *c_gpu = nullptr;

    cudaMalloc((void **)&a_gpu, SizeInBytes);
    cudaMalloc((void **)&b_gpu, SizeInBytes);
    cudaMalloc((void **)&c_gpu, SizeInBytes);

    cudaMemcpy(a_gpu, a, SizeInBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(b_gpu, b, SizeInBytes, cudaMemcpyHostToDevice);

    dim3 threads(512, 1);
    dim3 blocks((n + threads.x - 1) / threads.x, 1);

    VecMulKernel<<<blocks, threads>>>(a_gpu, b_gpu, c_gpu, n);

    cudaMemcpy(c, c_gpu, SizeInBytes, cudaMemcpyDeviceToHost);

    cudaFree(a_gpu);
    cudaFree(b_gpu);
    cudaFree(c_gpu);
}

void vec_add_cuda(float *a, float *b, float *c, int n)
{
    int SizeInBytes = n * sizeof(float);

    float *a_gpu = nullptr;
    float *b_gpu = nullptr;
    float *c_gpu = nullptr;

    cudaMalloc((void **)&a_gpu, SizeInBytes);
    cudaMalloc((void **)&b_gpu, SizeInBytes);
    cudaMalloc((void **)&c_gpu, SizeInBytes);

    cudaMemcpy(a_gpu, a, SizeInBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(b_gpu, b, SizeInBytes, cudaMemcpyHostToDevice);

    dim3 threads(512, 1);
    dim3 blocks((n + threads.x - 1) / threads.x, 1);

    VecAddKernel<<<blocks, threads>>>(a_gpu, b_gpu, c_gpu, n);

    cudaMemcpy(c, c_gpu, SizeInBytes, cudaMemcpyDeviceToHost);

    cudaFree(a_gpu);
    cudaFree(b_gpu);
    cudaFree(c_gpu);
}

// CPU 

void vec_add_cpu(float *a, float *b, float *c, int n)
{
    for (int i = 0; i < n; i++)
        c[i] = a[i] + b[i];
}