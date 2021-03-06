//
// Created by Hui Xie on 8/17/2018.
// Copyright (c) 2019 Hui Xie. All rights reserved.
//

#include "CollapseNet.h"

int main (int argc, char *argv[])
{
    cout<<"Test Dimensional Collapse in Convolution"<<endl;
    CPUAttr cpuAttr;
    cpuAttr.getCPUAttr();

#ifdef Use_GPU
    GPUAttr gpuAttr;
    gpuAttr.getGPUAttr();
    cout<<"Info: program use Cuda GPU."<<endl;
#else
    cout<<"Info: program use CPU, instead of GPU."<<endl;
#endif

    string netDir = "/home/hxie1/temp_netParameters/CollapseNet";

    CollapseNet net(netDir);
    AdamOptimizer optimizer(0.001,0.9,0.999);
    net.setOptimizer(&optimizer);


    if (isEmptyDir(net.getDir())) {
        net.build();
        net.initialize();
        // config network parameters;
        //net.setLearningRate(0.001);
        net.setLossTolerance(0.02);
        net.setBatchSize(20);
    }
    else{
        net.load();
    }
    net.printArchitecture();
    net.allocateOptimizerMem(optimizer.m_type);

    //  run network
    net.train();
    net.test();
    net.save();
    cout<< "=========== End of Test:  "<<net.getName() <<" ============"<<endl;
    return 0;
}
