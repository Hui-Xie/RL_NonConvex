//
// Created by Hui Xie on 10/1/18.
// Copyright (c) 2018 Hui Xie. All rights reserved.
//
#include "TensorKernels.h"
#include <cmath> //for pow()

__global__ void deviceInitialize(float *pData, const long N, const float value) {
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pData[index] = value;
        index += blockDim.x*gridDim.x;  //grid-stride loop
    }
}

// B = A', where B has a size M*N
__global__ void device2DMatrixTranspose(float* pA, float* pB, const long M, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    long totalN  = M*N;
    while (index < totalN){
        long m = index/N;
        long n = index%N; //index = m*N+n
        pB[index] = pA[n*M+m];
        index += blockDim.x*gridDim.x;  //grid-stride loop
    }
}

// C = A*B, where A has a size of M*K, B has a size of K*N, C will has a size of M*N
__global__ void device2DMatrixProduct(float* pA, float* pB, float* pC, const long M,const long N, const long K){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    long totalN  = M*N;
    while (index < totalN){
        pC[index] = 0.0f;
        long m = index/N;
        long n = index%N;
        for (long i=0; i<K; ++i){
            pC[index] += pA[m*K+i]*pB[i*N+n];
        }
        index += blockDim.x*gridDim.x;  //grid-stride loop
    }
}

// C = A*d, where C has a length of N, d is a scalar
__global__ void deviceTensorMultiply(float* pA, const float d, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pA[index] * d;
        index += blockDim.x*gridDim.x;
    }
}

// C = A .* B, hadamard product of A and B; A,B,C have same size
__global__ void deviceTensorHadamard(float* pA, float* pB, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pA[index] * pB[index];
        index += blockDim.x*gridDim.x;
    }
}

// C = A+B, where C has a length of N
__global__ void deviceTensorAdd(float* pA, float* pB, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pA[index] + pB[index];
        index += blockDim.x*gridDim.x;
    }
}

// C = A+d, where C has a length of N, d is a scalar
__global__ void deviceTensorAdd(float* pA, const float d, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pA[index] + d;
        index += blockDim.x*gridDim.x;
    }
}

// C = A-B, where C has a length of N
__global__ void deviceTensorSubtract(float* pA, float* pB, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pA[index] - pB[index];
        index += blockDim.x*gridDim.x;
    }
}

// C = A-d, where C has a length of N, d is a scalar
__global__ void deviceTensorSubtract(float* pA, const float d, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pA[index] - d;
        index += blockDim.x*gridDim.x;
    }
}

// C = A/d, where C has a length of N, d is a scalar
__global__ void deviceTensorDivide(float* pA, const float d, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pA[index]/d;
        index += blockDim.x*gridDim.x;
    }
}

// C = (A-d)^2, where d is a scalar, power is element-wise
__global__ void deviceTensorDiffPower(float* pA, const float d, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = pow(pA[index] - d, 2);
        index += blockDim.x*gridDim.x;
    }
}


//C = ln(A) natural logarithm
__global__ void deviceTensorLn(float* pA, float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = log(pA[index]);
        index += blockDim.x*gridDim.x;
    }
}

//C = exp(A) exponential
__global__ void deviceTensorExp(float* pA,float* pC, const long N){
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < N){
        pC[index] = exp(pA[index]);
        index += blockDim.x*gridDim.x;
    }
}

//C = flip(A)
__global__ void deviceTensorFlip(float* pA, const long N){
    long M = N/2;
    long index = threadIdx.x + blockIdx.x * blockDim.x;
    while (index < M){
        float temp = 0;
        temp = pA[index];
        pA[index] = pA[N-index-1];
        pA[N-index-1] = temp;
        index += blockDim.x*gridDim.x;
    }
}