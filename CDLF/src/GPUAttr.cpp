//
// Created by Hui Xie on 10/2/18.
// Copyright (c) 2019 Hui Xie. All rights reserved.
//

#include "GPUAttr.h"
#ifdef Use_GPU
   #include "cuda_runtime.h"
#endif
#include <iostream>

using namespace std;

int GPUAttr::m_numDevices = 0;
int GPUAttr::m_numSMs = 0;
int GPUAttr::m_maxThreadsPerBlock = 0;
int GPUAttr::m_blocksPerGrid = 0;
int GPUAttr::m_driverVersion = 0;


void cudaPrintError(){
#ifdef Use_GPU
    cudaError_t cudaError = cudaGetLastError();
    if (0 != cudaError){
        cout<<"Cuda error: "<<cudaError<< "; Error string = "<< cudaGetErrorString(cudaError)<<endl;
    }
#endif
}

GPUAttr::GPUAttr() {

}

GPUAttr::~GPUAttr() {

}

void GPUAttr::getGPUAttr() {
#ifdef Use_GPU
    cudaDeviceReset();

    cudaRuntimeGetVersion(&m_driverVersion);
    cout<<"GPU driver version ((1000 major + 10 minor):  "<< m_driverVersion <<endl;

    cudaGetDeviceCount(&m_numDevices);
    cout<<"This host has "<< m_numDevices <<" GPUs"<<endl;

    cudaDeviceGetAttribute(&m_numSMs, cudaDevAttrMultiProcessorCount, 0);
    cout << "m_numSMs = " << m_numSMs << endl;

    cudaDeviceGetAttribute(&m_maxThreadsPerBlock, cudaDevAttrMaxThreadsPerBlock, 0);
    cout << "m_maxThreadsPerBlock = " << m_maxThreadsPerBlock << endl;

    m_blocksPerGrid = 32 * m_numSMs;

    int deviceID = 0;
    if (cudaSuccess == cudaGetDevice(&deviceID)){
        cout<<"At beginning, currently use GPU device ID: "<<deviceID<<endl;
    }
    else{
        cudaPrintError();
    }
    cout<<"Program will use the GPU with maximum free memory in the following."<<endl;

    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp, deviceID);
    int m_computeCapabilityMajor = deviceProp.major;
    int m_computeCapabilityMinor = deviceProp.minor;
    std::printf("GPU Compute Capability: %d.%d\n", m_computeCapabilityMajor, m_computeCapabilityMinor);

#endif

}