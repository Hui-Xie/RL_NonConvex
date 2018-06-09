//
// Created by Sheen156 on 6/5/2018.
//

#ifndef RL_NONCONVEX_NET_H
#define RL_NONCONVEX_NET_H

#include "Layer.h"
#include <list>
#include <vector>
using namespace std;

class Net {
public:
    Net();
    ~Net();

    void setBatchSize(const int batchSize);
    void setLearningRate(const float learningRate);

    void forwardPropagate();
    void backwardPropagate();
    void addLayer(Layer* layer);
    void sgd(const float lr);

    //Notes: this layerWidthVector does not include LossLayer
    void buildNet(const vector<long> layerWidthVector);
    void initilize();
    void train(const int nIteration);


private:
    list<Layer*> m_layers;
    int m_batchSize;
    float m_learningRate;



};


#endif //RL_NONCONVEX_NET_H
