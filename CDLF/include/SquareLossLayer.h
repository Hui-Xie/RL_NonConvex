//
// Created by Hui Xie on 11/29/18.
// Copyright (c) 2019 Hui Xie. All rights reserved.

//

#ifndef CDLF_FRAMEWORK_SQUARELOSS_H
#define CDLF_FRAMEWORK_SQUARELOSS_H

#include "LossLayer.h"

/* Mean Square Loss Layer
 * L= lambda*(0.5/N)*\sum (x_i- g_i)^2
 * where g_i is the groundtruth distribution
 *       x_i is the output of previous layer, e.g. softmax;
 *       lambda is super parameter as weight for Mean square loss
 *       N is the number of total elements
 *
 * */

class SquareLossLayer  : public LossLayer {
public:
    SquareLossLayer(const int id, const string& name,  Layer *prevLayer, float lambda = 1.0);
    ~SquareLossLayer();

    float m_lambda;

    virtual  void saveStructLine(FILE* pFile);
    virtual  void printStruct();

private:
    virtual float lossCompute();
    virtual void  gradientCompute();
};




#endif //CDLF_FRAMEWORK_SQUARELOSS_H
