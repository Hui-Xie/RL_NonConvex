//
// Created by Hui Xie on 7/19/2018.
// Copyright (c) 2018 Hui Xie. All rights reserved.

#include <BiasLayer.h>
#include "statisTool.h"


BiasLayer::BiasLayer(const int id, const string& name, Layer* prevLayer): Layer(id,name, prevLayer->m_tensorSize){
   m_type = "BiasLayer";
   addPreviousLayer(prevLayer);
   m_pBTensor =   new Tensor<float>(prevLayer->m_tensorSize);
   m_pdBTensor =  new Tensor<float>(prevLayer->m_tensorSize);
}

BiasLayer::~BiasLayer() {
  if (nullptr != m_pBTensor) {
      delete m_pBTensor;
      m_pBTensor = nullptr;
  }
  if (nullptr != m_pdBTensor) {
      delete m_pdBTensor;
      m_pdBTensor = nullptr;
  }
}


void BiasLayer::initialize(const string& initialMethod){
  long N = m_pBTensor->getLength();
  generateGaussian(m_pBTensor, 0, sqrt(1.0/N));
}

void BiasLayer::zeroParaGradient(){
    if (nullptr != m_pdBTensor) m_pdBTensor->zeroInitialize();
}

//Y = X + b
void BiasLayer::forward(){
  *m_pYTensor = *m_prevLayer->m_pYTensor + *m_pBTensor;
}

// dL/dX = dL/dY    Where L is Loss
// dL/db = dL/dY
void BiasLayer::backward(bool computeW, bool computeX){
    if (computeW) {
        *m_pdBTensor += *m_pdYTensor;
    }
    if (computeX){
        *(m_prevLayer->m_pdYTensor) += *m_pdYTensor;
    }

}

void BiasLayer::updateParameters(const float lr, const string& method, const int batchSize){
    if ("sgd" == method){
        *m_pBTensor -=  (*m_pdBTensor)*(lr/batchSize);
    }
}

long BiasLayer::getNumParameters(){
     return m_pBTensor->getLength();
}

void BiasLayer::save(const string &netDir) {
    FILE * pFile = nullptr;
    string filename = "";

    string layerDir = netDir + "/" + to_string(m_id);
    createDir(layerDir);

    filename= layerDir + "/B.csv";
    m_pBTensor->save(filename);
}

void BiasLayer::load(const string &netDir) {
    FILE * pFile = nullptr;
    string filename = "";

    string layerDir = netDir + "/" + to_string(m_id);
    if (!dirExist(layerDir)){
        initialize("Xavier");
        return;
    }
    else{
        filename= layerDir + "/B.csv";
        m_pBTensor->load(filename);
    }
}

void BiasLayer::saveStructLine(FILE *pFile) {
    //const string tableHead= "ID, Type, Name, PreviousLayerIDs, OutputTensorSize, FilterSize, NumFilter, FilterStride(k), StartPosition, \r\n"
    fprintf(pFile, "%d, %s, %s, %d, %s, %s, %d, %d, %s, \r\n", m_id, m_type.c_str(), m_name.c_str(), m_prevLayer->m_id,
            vector2Str(m_tensorSize).c_str(), "{}", 0, 0, "{}");
}

void BiasLayer::printStruct(const int layerIndex) {
    printf("Layer%03d, Name=%s, Type=%s, id=%d, PrevLayer=%s, OutputSize=%s; \n",
           layerIndex, m_name.c_str(),m_type.c_str(), m_id,  m_prevLayer->m_name.c_str(), vector2Str(m_tensorSize).c_str());
}
