//
// Created by Hui Xie on 7/28/2018.
//

#ifndef RL_NONCONVEX_MAXPOOLINGLAYER_H
#define RL_NONCONVEX_MAXPOOLINGLAYER_H

#include "Layer.h"

/*
 * Notes: max-pooling can simply be replaced by a convolutional layer with increased stride
 *      without loss in accuracy on several image recognition benchmarks.
 *
 * */

class MaxPoolingLayer :  public Layer {
public:
    MaxPoolingLayer(const int id, const string& name, const vector<long>& filterSize, Layer* prevLayer,
                     const int stride=1);
    ~MaxPoolingLayer();

    void constructY();


    virtual  void initialize(const string& initialMethod);
    virtual  void zeroParaGradient();
    virtual  void forward();
    virtual  void backward();
    virtual  void updateParameters(const float lr, const string& method, const int batchSize =1);

private:
    int m_stride;
    int m_OneFilterN;
    vector<long> m_filterSize;

};


#endif //RL_NONCONVEX_MAXPOOLINGLAYER_H