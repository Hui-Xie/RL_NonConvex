//
// Created by Hui Xie on 8/17/2018.
// Copyright (c) 2018 Hui Xie. All rights reserved.
//

#include "Conv4DNet.h"

int main (int argc, char *argv[])
{
    cout<<"Test 4D Convolution"<<endl;

    Conv4DNet net;
    net.build();

    // config network parameters;
    net.setLearningRate(0.001);
    net.setLossTolerance(0.02);
    net.setBatchSize(20);
    net.printArchitecture();

    //  run network
    net.initialize();
    net.train();
    net.test();
    cout<< "=========== End of ConvolutionLayer Test ============"<<endl;
    return 0;
}