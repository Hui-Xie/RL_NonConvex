//
// Created by Hui Xie on 6/6/2018.
// Copyright (c) 2018 Hui Xie. All rights reserved.

#ifndef RL_NONCONVEX_LOSSLAYER_H
#define RL_NONCONVEX_LOSSLAYER_H

#include "Layer.h"
#include "Tensor.h"

//LossLayer is an abstract class.
//LossLayer has no learning parameters.

class LossLayer : public Layer {
public:
    LossLayer(const int id, const string& name, Layer *prevLayer);
    ~LossLayer();

    float getLoss();

    virtual  void initialize(const string& initialMethod);
    virtual  void zeroParaGradient();
    virtual  void forward();
    virtual  void backward(bool computeW);
    virtual  void updateParameters(const float lr, const string& method, const int batchSize=1);

    virtual float lossCompute()=0;
    virtual void  gradientCompute()=0;
    virtual void printGroundTruth()=0;
    float m_loss;

    Tensor<float>* m_pGroundTruth;

    void setGroundTruth( const Tensor<float>& groundTruth);
    void setGroundTruth( const Tensor<unsigned  char>& groundTruth);
    virtual  long getNumParameters();

};


#endif //RL_NONCONVEX_LOSSLAYER_H