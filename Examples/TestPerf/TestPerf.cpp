//
// Created by Hui Xie on 8/13/2018.
//
#include "PerfNet.h"

int main (int argc, char *argv[])
{
    CPUAttr cpuAttr;
    cpuAttr.getCPUAttr();

#ifdef Use_GPU
    GPUAttr gpuAttr;
    gpuAttr.getGPUAttr();
    cout<<"Info: program use Cuda GPU."<<endl;
#else
    cout<<"Info: program use CPU, instead of GPU."<<endl;
#endif


    PerfNet net("/home/hxie1/temp_netParameters/PerfNet");
    AdamOptimizer optimizer(0.001,0.9,0.999);
    net.setOptimizer(&optimizer);

    if (isEmptyDir(net.getDir())) {
        net.build();
        net.initialize();
        //net.setLearningRate(0.001);
        net.setLossTolerance(0.02);
        net.setBatchSize(20);
    }
    else{
        net.load();
    }
    net.setUnlearningLayerID(24);
    net.printArchitecture();
    net.allocateOptimizerMem(optimizer.m_type);
    net.save();

    //  run network
    net.train();
    net.test();
    net.save();
    cout<< "=========== End of Test:  "<< net.getName() <<" ============"<<endl;
    return 0;
}