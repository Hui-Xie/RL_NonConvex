//
// Created by Hui Xie on 11/29/18.
// Copyright (c) 2018 Hui Xie. All rights reserved.

//

#ifndef CDLF_FRAMEWORK_SQUARELOSS_H
#define CDLF_FRAMEWORK_SQUARELOSS_H

#include "LossLayer.h"

/* L= lambda*0.5*\sum (x_i- g_i)^2
 * where g_i is the groundtruth distribution
 *       x_i is the output of previous layer, e.g. softmax;
 *       lambda is super parameter as weight for square loss
 * */

class SquareLoss  : public LossLayer {
public:
    SquareLoss(const int id, const string& name,  Layer *prevLayer, float lambda = 1.0);
    ~SquareLoss();
    virtual void  printGroundTruth();

    float m_lambda;

    virtual  void saveStructLine(FILE* pFile);
    virtual  void printStruct(const int layerIndex);

private:
    virtual float lossCompute();
    virtual void  gradientCompute();


};


#endif //CDLF_FRAMEWORK_SQUARELOSS_H