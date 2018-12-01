//
// Created by Hui Xie on 6/7/2018.
// Copyright (c) 2018 Hui Xie. All rights reserved.

#include <ReLU.h>

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
    for (long i=0; i< N; ++i){
       if (X.e(i) >= 0 ) Y.e(i) = X.e(i);
       else Y.e(i) = 0;
    }
}
void ReLU::backward(bool computeW, bool computeX){
    if (computeX){
        Tensor<float>& dY = *m_pdYTensor;
        Tensor<float>& dX = *m_prevLayer->m_pdYTensor;
        Tensor<float>& X = *m_prevLayer->m_pYTensor;
        long N = dY.getLength();
        for(long i=0; i< N; ++i){
            if (X.e(i) >= 0) dX.e(i) += dY.e(i);
            // all dX.e(i) = 0 in zeroDYTensor() method.
        }
    }
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

void ReLU::save(const string &netDir) {
   //null
}

void ReLU::load(const string &netDir) {
  //null
}

void ReLU::saveStructLine(FILE *pFile) {
    //const string tableHead= "ID, Type, Name, previousLayerIDs, outputTensorSize, filterSize, numFilter, FilterStride, startPosition, \r\n";
    fprintf(pFile, "%d, %s, %s, %d, %s, %s, %d, %d, %s, \r\n", m_id, m_type.c_str(), m_name.c_str(), m_prevLayer->m_id,
            vector2Str(m_tensorSize).c_str(), "{}", 0, 0, "{}");
}

void ReLU::printStruct(const int layerIndex) {
    printf("Layer%03d, Name=%s: (%s, id=%d): PrevLayer=%s, OutputSize=%s; \n",
           layerIndex, m_name.c_str(),m_type.c_str(), m_id,  m_prevLayer->m_name.c_str(), vector2Str(m_tensorSize).c_str());
}
