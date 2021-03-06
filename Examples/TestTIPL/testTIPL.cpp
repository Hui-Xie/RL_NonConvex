//
// Created by Hui Xie on 8/22/18.
// Copyright (c) 2018. All rights reserverd.

#include "TIPLIO.h"

//const string inputFilename = "/Users/hxie1/temp/BRATS_001.nii";
//const string outputFilename = "/Users/hxie1/temp/BRATS_001_Output.nii";

void printUsage(char* argv0){
    cout<<"Test TIPL image:"<<endl;
    cout<<"This interface does not support compress image file eg. gz file, and 4D file."<<endl;
    cout<<"TIPL always convert NifTI file into RAS orientation."<<endl;
    cout<<"Usage: "<<endl;
    cout<<argv0<<" fullPathInputFileNane fullPathOutputFilename"<<endl;
}


int main(int argc, char *argv[]){

    if (3 != argc){
        cout<<"Error: input parameter error."<<endl;
        printUsage(argv[0]);
        return -1;
    }

    const string inputFilename = argv[1];
    const string outputFilename = argv[2];

    CPUAttr cpuAttr;
    cpuAttr.getCPUAttr();


#ifdef Use_GPU
    GPUAttr gpuAttr;
    gpuAttr.getGPUAttr();
    cout<<"Info: program use Cuda GPU."<<endl;
#else
    cout<<"Info: program use CPU, instead of GPU."<<endl;
#endif

    TIPLIO<float, 3> imageIO;

    Tensor<float>* pImage = nullptr;
    int result = imageIO.readNIfTIFile(inputFilename, pImage);
    if (0 != result){
        return -1;
    }

    //change value of pImage,
    vector<int> tensorSize = pImage->getDims();
    vector<int> halfTensorSize = tensorSize /2;
    for(int i=halfTensorSize[0]-20;i<halfTensorSize[0]+20;++i)
        for(int j=halfTensorSize[1]-20; j<halfTensorSize[1]+20;++j)
            for(int k=halfTensorSize[2]-20;k<halfTensorSize[2]+20;++k){
                pImage->e(i,j,k) = 0;  //dig a hole in the middle of brain.
            }

    imageIO.write3DNIfTIFile(pImage, {0,0,0}, outputFilename);

    if (nullptr != pImage){
        delete pImage;
        pImage = nullptr;
    }
    cout<<"================End of TIPL Read Writer==========="<<endl;

    return 0;
}

