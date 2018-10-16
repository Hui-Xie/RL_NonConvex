//
// Created by Hui Xie on 6/7/2018.
// Copyright (c) 2018 Hui Xie. All rights reserved.

#include "ReLU.h"

//ReLU has just one previous layer.

ReLU::ReLU(const int id, const string& name,Layer* prevLayer): Layer(id,name, prevLayer->m_tensorSize){
    m_type = "ReLU";
    addPreviousLayer(prevLayer);
}

ReLU::~ReLU(){

}

// Y = X if X>=0; Y =0 else;
// dL/dx = dL/dy * dy/dx = dL/dy if X>=0; 0 if X < 0
void ReLU::forward(){
    Tensor<float>& Y = *m_pYTensor;
    Tensor<float>& X = *m_prevLayer->m_pYTensor;
    long N = Y.getLength();
#ifdef Use_GPU
    cudaRelu(X.getData(),Y.getData(),N);
#else
    for (long i=0; i< N; ++i){
       if (X.e(i) >= 0 ) Y.e(i) = X.e(i);
       else Y.e(i) = 0;
    }
#endif
}
void ReLU::backward(bool computeW){
    Tensor<float>& dY = *m_pdYTensor;
    Tensor<float>& dX = *m_prevLayer->m_pdYTensor;
    Tensor<float>& X = *m_prevLayer->m_pYTensor;
    long N = dY.getLength();
#ifdef Use_GPU
    cudaReluDerivative(X.getData(),dY.getData(),dX.getData(),N);
#else
    for(long i=0; i< N; ++i){
        if (X.e(i) >= 0) dX.e(i) += dY.e(i);
        // all dX.e(i) = 0 in zeroDYTensor() method.
    }
#endif
}
void ReLU::initialize(const string& initialMethod){
    //null
}

void ReLU::zeroParaGradient(){
    //null
}

void ReLU::updateParameters(const float lr, const string& method, const int batchSize) {
    //null
}

long  ReLU::getNumParameters(){
    return 0;
}