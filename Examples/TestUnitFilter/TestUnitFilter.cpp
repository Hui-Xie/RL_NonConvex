//
// Created by Hui Xie on 6/5/2018.
//

#include "UnitFilterNet.h"
#include "LossConvexExample1.h"
#include <sstream>

using namespace std;

void printUsage(){
   //null
}

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

    UnitFilterNet net("UnitfilterNet");

    net.build();
    net.printArchitecture();

    net.setLearningRate(0.01);
    net.setLossTolerance(0.02);
    net.setBatchSize(20);
    net.initialize();

    net.train();
    net.test();
    cout<< "=========== End of Test:  "<<net.getName() <<" ============"<<endl;
    return 0;

}
