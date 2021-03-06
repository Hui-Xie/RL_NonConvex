//
// Created by Hui Xie on 6/5/2018.
// Copyright (c) 2019 Hui Xie. All rights reserved.

#ifndef RL_FEEDFORWARDNET_H
#define RL_FEEDFORWARDNET_H

#include "Net.h"
using namespace std;

class FeedForwardNet : public Net {
public:
    FeedForwardNet(const string& saveDir);
    ~FeedForwardNet();

    void forwardPropagate();
    void backwardPropagate(bool computeW);
    void optimize();

    void allocateOptimizerMem(const string method);
    void freeOptimizerMem();

    //void initializeLRs(const float lr);
    //void updateLearingRates(const float deltaLoss);

    virtual void build() = 0;
    virtual void train() = 0;
    virtual float test() = 0;
};


#endif //RL_FEEDFORWARDNET_H
