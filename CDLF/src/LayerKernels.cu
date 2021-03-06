//
// Created by Hui Xie on 10/10/18.
// Copyright (c) 2019 Hui Xie. All rights reserved.

//

#include "LayerKernels.h"
#include "TensorKernels.h"
#include <stdio.h>  //# for printf debug

/*
__global__ void deviceSigmoidDerivative(const float* __restrict__  pX, const float* __restrict__  pdY, float* pdX, const int k, const int N){
    int i = threadIdx.x + blockIdx.x * blockDim.x; //i: thread index
    while (i < N){
        float  expx = exp(pX[i]);
        pdX[i] += pdY[i]*k*expx/pow(1+expx,2);
        i += blockDim.x*gridDim.x;
    }
}

__global__ void deviceSigmoid(const float* __restrict__  pX, float* pY, const int k, const int N){
    int i = threadIdx.x + blockIdx.x * blockDim.x; //i: thread index
    while (i < N){
        float exp_x = exp(-pX[i]);
        pY[i] = k/(1+exp_x);
        i += blockDim.x*gridDim.x;
    }

}


__global__ void deviceCrossEntropyGradient(const float* __restrict__  pX, const float* __restrict__  pGTX, float* pdX, const float epsilon, const int N){
    int i = threadIdx.x + blockIdx.x * blockDim.x; //i: thread index
    while (i < N){
        if (0 != pX[i]){
            pdX[i] -= pGTX[i]/pX[i];
        }
        else{
            pdX[i] -= pGTX[i]/epsilon;
        }
        i += blockDim.x*gridDim.x;
    }
}


//C = A where A and C has different value type
__global__ void deviceElementCopy(const unsigned char* __restrict__  pA,float* pC, const int N){
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    while (i < N){
        pC[i] = (float)pA[i];
        i += blockDim.x*gridDim.x;
    }
}

//C = A if A>=0; C =0 else
__global__ void deviceRelu(const float* __restrict__  pA,float* pC, const int N){
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    while (i < N){
        if (pA[i]>0){
            pC[i] = pA[i];
        }
        else{
            pC[i] = 0;
        }
        i += blockDim.x*gridDim.x;
    }
}

// dL/dx = dL/dy * dy/dx = dL/dy if X>=0; 0 if X < 0
__global__ void deviceReluDerivative(const float* __restrict__  pX, const float* __restrict__  pdY, float* pdX, const int N){
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    while (i < N){
        if (pX[i]>= 0){
            pdX[i] = pdY[i];
        }
        else{
            pdX[i] = 0;
        }
        i += blockDim.x*gridDim.x;
    }

}

__global__ void deviceSoftmax(const float* __restrict__  pX, float* pY, const int nSoftmax, const int N){
    int j = threadIdx.x + blockIdx.x * blockDim.x; // j is thread index
    while (j < N){
        float sumExpX = 1e-8;;
        for (int i=0; i< nSoftmax; ++i){
            sumExpX += exp(pX[i*N+j]);
        }
        for (int i=0; i< nSoftmax; ++i){
            pY[i*N+j] = exp(pX[i*N+j])/sumExpX;
        }
        j += blockDim.x*gridDim.x;
    }
}

__global__ void deviceSoftmaxDerivative(const float* __restrict__  pX,const float* __restrict__  pdY, float* pdX, const int nSoftmax, const int N){
    int j = threadIdx.x + blockIdx.x * blockDim.x; // j is thread index
    while (j < N){
        float sumExpX = 1e-8;
        for (int i=0; i< nSoftmax; ++i){
            sumExpX += exp(pX[i*N+j]);
        }
        float sumExpX2 = sumExpX*sumExpX;

        // \sum(dL/dy_j*exp(x_j)
        float dyDotExpX = 0;
        for(int i=0; i< nSoftmax; ++i){
            dyDotExpX += pdY[i*N+j]*exp(pX[i*N+j]);
        }

        for(int i=0; i< nSoftmax; ++i){
            pdX[i*N+j] += exp(pX[i*N+j])*(pdY[i*N+j]*sumExpX-dyDotExpX)/sumExpX2;
        }

        j += blockDim.x*gridDim.x;
    }
}

//C = A*F in convolution
__global__ void deviceConvLayerForward(const float* pA, const int* pADimsSpan, const float* pF, const int* pFDimsSpan, const int filterSize, const int NFilter,
                                       const int stride, float* pC, const int* pCDimsSpan, const int* pNonZeroIndex, const int CDimsSize, const int N){
    int t = threadIdx.x + blockIdx.x * blockDim.x; //t indicates thread index
    while (t < N){
        //generate C index;
        extern __shared__ int sharedMem[];//byteLengthSharedMem=CDimsSize*sizeof(int)+ filterSize*sizeof(int);
        int* pCIndex= sharedMem;
        int* pAIndex = (int*) &pCIndex[CDimsSize]; //// all pointers of shared memory does not need to delete.

        int n = t;
        for (int i = 0; i <CDimsSize; ++i) {
            pCIndex[i] = n / pCDimsSpan[i];
            n -= pCIndex[i] * pCDimsSpan[i];
        }

        // generate A index
        for (int i=0; i< filterSize; ++i){
            pAIndex[0] = 0;
        }
        for (int i=0; i< CDimsSize; ++i){
            pAIndex[pNonZeroIndex[i]] = pCIndex[i]*stride;
        }

        float* pSubA=  new float[NFilter];
        float* pHadamard = new float[NFilter];

        deviceSubTensorFromTopLeft<<<1, NFilter, filterSize*sizeof(int)>>>(pA, pADimsSpan, pAIndex, pFDimsSpan, filterSize, 1, pSubA, NFilter);
        //__syncthreads();

        deviceTensorHadamard<<<1, NFilter>>>(pSubA, pF, pHadamard, NFilter);
        //__syncthreads();
        float sum =0;
        for(int i=0; i<NFilter; ++i){
            sum += pHadamard[i];
        }
        pC[t] = sum;

        delete[] pSubA;
        delete[] pHadamard;

        t += blockDim.x*gridDim.x;
    }

}

*/
