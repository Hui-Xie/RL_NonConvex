//
// Created by Hui Xie on 12/11/18.
// Copyright (c) 2018 Hui Xie. All rights reserved.

//

#include "HNRadiomicsNet.h"

void printUsage(char* argv0){
    cout<<"Train Head&Neck Radiomics Network:"<<endl;
    cout<<"Usage: "<<endl;
    cout<<argv0<<"<netDir> <fullPathOfRadiomicsDataDir>  <learningRate>"<<endl;
    cout<<"For examples: "<<endl;
    cout<<argv0<<" /home/hxie1/temp_netParameters /home/hxie1/data/HeadNeckSCC/ExtractData  1"<<endl;
    cout<<argv0<<" /Users/hxie1/temp_netParameters  /Users/hxie1/data/HeadNeckSCC/ExtractData 1"<<endl;
}


int main(int argc, char *argv[]){
    printCurrentLocalTime();
    if (4 != argc){
        cout<<"Error: input parameter error."<<endl;
        printUsage(argv[0]);
        return -1;
    }

    const string netDir = argv[1];
    const string dataDir = argv[2];
    const float learningRate = stof(argv[3]);

    CPUAttr cpuAttr;
    cpuAttr.getCPUAttr();

#ifdef Use_GPU
    GPUAttr gpuAttr;
    gpuAttr.getGPUAttr();
    cout<<"Info: program use Cuda GPU."<<endl;
#else
    cout<<"Info: program use CPU, instead of GPU."<<endl;
#endif

    HNRadiomicsNet net("HNSCC_matrix", netDir);
    //HNRadiomicsNet net("HNSCC_convV", netDir);
    cout<<"=========================================="<<endl;
    cout<<"Info: this "<<net.getName() <<" net."<<endl;
    cout<<"=========================================="<<endl;

    if (!isEmptyDir(net.getDir())) {
        net.load();
    }
    else{
        cout<<"Error: program can not load net."<<endl;
        return -2;
    }
    net.defineAssemblyLoss();
    net.printArchitecture();
    net.setLearningRate(learningRate);

    //for one sample training
    net.setOneSampleTrain(true);

    HNDataManager dataMgr(dataDir);
    net.m_pDataMgr = &dataMgr;

    int epoch= 15000;
    //int epoch = 1;

    for (int i=0; i<epoch; ++i){
        net.train();
        if (net.getOneSampleTrain()){
            cout<<"One Sample Training: "<<endl;
        }
        else{
            net.save();
            net.test();
        }
        cout<<"Epoch_"<<i<<": "<<" mean Assembly Loss for each test sample = "<< net.m_loss <<endl;
        cout<<"Epoch_"<<i<<": "<<" mean dice coefficient =   "<< net.m_dice <<endl;
        cout<<"Epoch_"<<i<<": "<<" mean True Positive Rate =   "<< net.m_TPR <<endl;

    }
    cout<< "=========== End of Test:  "<<net.getName() <<" ============"<<endl;
    return 0;
}