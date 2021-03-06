//
// Created by Hui Xie on 1/19/2019.
// Copyright (c) 2019 Hui Xie. All rights reserved.

#ifndef CDLF_FRAME_MATRIXCONVERT_H
#define CDLF_FRAME_MATRIXCONVERT_H

#include "Layer.h"
#include <map>

//pure virtual class

class MatrixLayer :  public Layer{
public:
    MatrixLayer(const int id, const string& name, Layer* prevLayer, const vector<int>& filterSize, const vector<int>& tensorSize);
    ~MatrixLayer();

    Tensor<float>*  m_pW;
    Tensor<float>*  m_pB;
    Tensor<float>*  m_pdW;
    Tensor<float>*  m_pdB;

    // Adam
    Tensor<float>*  m_pWM;  //1st moment
    Tensor<float>*  m_pBM;
    Tensor<float>*  m_pWR;
    Tensor<float>*  m_pBR; //2nd moment


    virtual  void initialize(const string& initialMethod);
    virtual  void zeroParaGradient();
    virtual  void averageParaGradient(const int batchSize);


    virtual void allocateOptimizerMem(const string method);
    virtual void freeOptimizerMem();

    //virtual  void initializeLRs(const float lr);
    //virtual  void updateLRs(const float deltaLoss);
    virtual  void updateParameters(Optimizer* pOptimizer);

    virtual  int getNumParameters();

    virtual  void save(const string& netDir);
    virtual  void load(const string& netDir);
    virtual  void saveStructLine(FILE* pFile);
    virtual  void printStruct();
};

#endif //CDLF_FRAME_MATRIXCONVERT_H