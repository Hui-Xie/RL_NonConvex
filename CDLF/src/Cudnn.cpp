
#include <Cudnn.h>

Cudnn::Cudnn(Layer* pLayer){
    m_pLayer = pLayer;
    d_pX = nullptr;
    d_pY = nullptr;
    d_pdX = nullptr;
    d_pdY = nullptr;
    chooseIdleGPU();
    checkCUDNN(cudnnCreate(&m_cudnnContext));
    checkCUDNN(cudnnCreateTensorDescriptor(&m_xDescriptor));
    checkCUDNN(cudnnCreateTensorDescriptor(&m_yDescriptor));

}

Cudnn::~Cudnn(){
   freeDeviceX();
   freeDeviceY();
   freeDevicedX();
   freeDevicedY();
   cudnnDestroyTensorDescriptor(m_xDescriptor);
   cudnnDestroyTensorDescriptor(m_yDescriptor);
   cudnnDestroy(m_cudnnContext);
}

void Cudnn::setXDescriptor() {
    vector<int> & tensorSize = m_pLayer->m_prevLayer->m_tensorSize;

    //The first dimension of the tensor defines the batch size n, and the second dimension defines the number of features maps c.
    int nbDims = tensorSize.size()+2;

    int* dimA = new int[nbDims];
    dimA[0] = 1;
    dimA[1] = 1;
    for (int i=2; i< nbDims; ++i){
        dimA[i]  = tensorSize[i-2];
    }

    int* strideA = new int [nbDims];  //span in each dimension. It is a different concept with filter-stride in convolution.
    dimA2SpanA(dimA, nbDims, strideA);

    checkCUDNN(cudnnSetTensorNdDescriptor(m_xDescriptor, CUDNN_DATA_FLOAT, nbDims, dimA, strideA));

   // cout<<"In "<<m_pLayer->m_name<<": ";
    //cout<<"xDescriptor: "<<array2Str(dimA, nbDims)<<endl;

    delete[] dimA;
    delete[] strideA;
}



void Cudnn::allocateDeviceX() {
    freeDeviceX();

    const int xSize = length(m_pLayer->m_prevLayer->m_tensorSize);
    cudaMalloc(&d_pX, xSize*sizeof(float));
    cudaMemcpy(d_pX, m_pLayer->m_prevLayer->m_pYTensor->getData(), xSize*sizeof(float), cudaMemcpyHostToDevice);
}

void Cudnn::allocateDeviceY(bool copyComputedY) {
    freeDeviceY();

    const int ySize = length(m_pLayer->m_tensorSize);
    cudaMalloc(&d_pY, ySize*sizeof(float));
    if (copyComputedY){
        cudaMemcpy(d_pY, m_pLayer->m_pYTensor->getData(), ySize*sizeof(float), cudaMemcpyHostToDevice);
    }
    else{
       cudaMemset(d_pY, 0, ySize*sizeof(float));
    }
}


void Cudnn::allocateDevicedX() {
    freeDevicedX();

    const int xSize = length(m_pLayer->m_prevLayer->m_tensorSize);
    cudaMalloc(&d_pdX, xSize*sizeof(float));
    cudaMemset(d_pdX, 0, xSize*sizeof(float));
}

void Cudnn::allocateDevicedY() {
    freeDevicedY();

    const int ySize = length(m_pLayer->m_tensorSize);
    cudaMalloc(&d_pdY, ySize*sizeof(float));
    cudaMemcpy(d_pdY, m_pLayer->m_pdYTensor->getData(), ySize*sizeof(float), cudaMemcpyHostToDevice);
}

void Cudnn::freeDeviceX() {
    if (nullptr != d_pX) {
        cudaFree(d_pX);
        d_pX = nullptr;
    }
}

void Cudnn::freeDeviceY() {
    if (nullptr != d_pY) {
        cudaFree(d_pY);
        d_pY = nullptr;
    }
}

void Cudnn::freeDevicedX() {
    if (nullptr != d_pdX) {
        cudaFree(d_pdX);
        d_pdX = nullptr;
    }
}

void Cudnn::freeDevicedY() {
    if (nullptr != d_pdY) {
        cudaFree(d_pdY);
        d_pdY = nullptr;
    }
}

void Cudnn::checkDeviceTensor(float *d_pA, const vector<int>  tensorSize) {
    Tensor<float> A(tensorSize);
    const int N = length(tensorSize);
    cudaMemcpy(A.getData(), d_pA, N* sizeof(float), cudaMemcpyDeviceToHost);
    A.print();
}

int Cudnn::chooseIdleGPU() {
    const int numDevices = GPUAttr::m_numDevices;
    int idleGPU = 0;
    if (1 != numDevices){
        GPUUsage* gpuUsages = new GPUUsage[numDevices];
        for (int i=0; i<numDevices; ++i){
            cudaSetDevice(i);
            gpuUsages[i].m_deviceID = i;
            cudaMemGetInfo(&gpuUsages[i].m_freeMem, &gpuUsages[i].m_totalMem);

            // for debug use.
            //printf("GPU_ID =%d, freeMem=%ld bytes, totalMem=%ld bytes\n", i, gpuUsages[i].m_freeMem, gpuUsages[i].m_totalMem);
        }

        size_t maxFreeMem = gpuUsages[0].m_freeMem;
        for (int i=1; i<numDevices; ++i){
            if (gpuUsages[i].m_freeMem > maxFreeMem){
                idleGPU = i;
                maxFreeMem = gpuUsages[i].m_freeMem;
            }
        }
        delete[] gpuUsages;
    }
    cudaSetDevice(idleGPU);
    return idleGPU;

}
















