//
// Created by Hui Xie on 12/11/18.
// Copyright (c) 2018 Hui Xie. All rights reserved.

//

#include "MNIST.h"
#include "MnistAutoEncoder.h"
#include <cstdio>

void printUsage(char* argv0){
    cout<<"Train MNIST Dataset AutoEncoder:"<<endl;
    cout<<"Usage: "<<endl;
    cout<<argv0<<"<netDir> <fullPathOfMnistDataDir> "<<endl;
    cout<<"For examples: "<<endl;
    cout<<argv0<<" /home/hxie1/temp_netParameters /home/hxie1/Projects/mnist "<<endl;
}


int main(int argc, char *argv[]){
    printCurrentLocalTime();
    if (3 != argc){
        cout<<"Error: input parameter error."<<endl;
        printUsage(argv[0]);
        return -1;
    }

    const string netDir = argv[1];
    const string mnistDir = argv[2];

    CPUAttr cpuAttr;
    cpuAttr.getCPUAttr();

#ifdef Use_GPU
    GPUAttr gpuAttr;
    gpuAttr.getGPUAttr();
    cout<<"Info: program use Cuda GPU."<<endl;
#else
    cout<<"Info: program use CPU, instead of GPU."<<endl;
#endif

    // Load MNIST Data
    bool onlyTestSet = false;
    MNIST mnist(mnistDir, onlyTestSet);
    mnist.loadData();

    //Load Mnist Net
    MnistAutoEncoder net("MnistAutoEncoder", netDir);
    if (!isEmptyDir(net.getDir())) {
        net.load();  //at Dec 11th,2018, the trained G net has an accuracy of 97.1%
    }
    else{
        cout<<"Error: program can not load a trained Mnist net."<<endl;
        return -2;
    }
    net.printArchitecture();
    net.setLearningRate(0.001);
    net.setUnlearningLayerID(20);  // 18 is the FC2 behind the Softmax of original G net.

    // choose an initial digit file and make sure it is predicted correctly
    srand (time(NULL));
    bool bPredictCorrect = false;
    Tensor<float> inputTensor;
    Tensor<float> groundTruth;
    int label; // correct label, which is different with target.
    while (!bPredictCorrect){
        long index = rand() % 10000;
        mnist.getTestImageAndLabel(index, inputTensor, label);
        net.constructGroundTruth(label, groundTruth);
        net.m_groundTruth = groundTruth;
        if (net.predict(inputTensor) == label){
            bPredictCorrect = true;
        }
    }
    net.m_originTensor = inputTensor;

    string originFile = to_string(label)+".txt";
    originFile = advDataDir +"/"+ originFile;
    inputTensor.save(originFile, true);
    printf("Info: output original file: %s with label of %d. \n", originFile.c_str(), label);

    vector<int> targetVec= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(vector<int>::iterator it= targetVec.begin(); it != targetVec.end(); ++it){
        if (label == *it){
            targetVec.erase(it);
            break;
        }
    }

    for(int i=0; i< targetVec.size(); ++i){
        const int target = targetVec[i];
        bool bFoundAdversary = false;
        net.m_adversaryTensor = net.m_originTensor; // re-assign original input Tensor
        net.constructGroundTruth(target, groundTruth);
        net.m_groundTruth = groundTruth;  //targeted groundtruth, not the correct label.
        int nCount = 0;
        int MaxCount = 1000;
        while(nCount < MaxCount){
            net.train();
            // generate gardient files for specific target
            if (bGenerateGradientFiles){
                string gradientFile = to_string(label)+"-Ad"+ to_string(target)+"-G"+to_string(nCount)+".txt";
                gradientFile = advDataDir +"/" + gradientFile;
                net.saveInputDY(gradientFile);
            }

            ++nCount;
            if (net.predict(net.m_adversaryTensor ) == target){
               bFoundAdversary = true;
               break;
            }
        }

        if (bFoundAdversary){
            string advFile = to_string(label)+"-Ad"+ to_string(target)+".txt";
            advFile = advDataDir +"/"+ advFile;
            net.m_adversaryTensor.save(advFile, true);
            printf("After %d back propagation iterations, an adversarial file output at: %s\n", nCount, advFile.c_str());
        }
        else{
            printf("Infor: program can not find adversary for target %d, basing on original digit: %d\n", target, label);
        }

    }

    cout<< "=========== End of Generating Adversary by a trained network"<<net.getName() <<" ============"<<endl;
    return 0;
}