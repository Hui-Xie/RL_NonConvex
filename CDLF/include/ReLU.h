//
// Created by Hui Xie on 6/7/2018.
// Copyright (c) 2019 Hui Xie. All rights reserved.

#ifndef CDLF_FRAME_RELU_H
#define CDLF_FRAME_RELU_H
#include "Layer.h"

/*  parameter ReLU
 *
 *  y = x when x>= k;
 *  y = 0 when x<k;
 */

class ReLU : public Layer {
public:
    ReLU(const int id, const string& name,Layer* prevLayer, const vector<int>& tensorSize, const float k = 0);
    ~ReLU();

    virtual  void initialize(const string& initialMethod);
    virtual  void zeroParaGradient();
    virtual  void averageParaGradient(const int batchSize);
    virtual  void forward();
    virtual  void backward(bool computeW, bool computeX = true);


    virtual  void initializeLRs(const float lr);
    virtual  void updateLRs(const float deltaLoss);
    virtual  void updateParameters(Optimizer* pOptimizer);

    virtual  int getNumParameters();
    virtual  void save(const string& netDir);
    virtual  void load(const string& netDir);
    virtual  void saveStructLine(FILE* pFile);
    virtual  void printStruct();

    float m_k;
};


#endif //CDLF_FRAME_RELU_H
