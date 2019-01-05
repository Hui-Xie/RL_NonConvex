//
// Created by Hui Xie on 1/5/19.
// Copyright (c) 2018 Hui Xie. All rights reserved.

//

#ifndef CDLF_FRAMEWORK_ITKDATAMANAGER_H
#define CDLF_FRAMEWORK_ITKDATAMANAGER_H

#include <string>
#include <vector>
#include "Tensor.h"
#include "ITKImageIO.h"

using namespace std;

class ITKDataManager {
public:
    ITKDataManager(const string dataSetDir);
    ~ITKDataManager();

    string m_dataSetDir;
    string m_trainImagesDir;
    string m_trainLabelsDir;
    string m_testImagesDir;
    string m_testLabelsDir;

    string m_outputLabelsDir;

    vector<string> m_testImagesVector;
    vector<string> m_trainImagesVector;
    int m_NTrainFile;
    int m_NTestFile;


    void readTrainImageFile(const int index, Tensor<float>*& pImage);
    void readTestImageFile(const int index, Tensor<float>*& pImage);
    void readTrainLabelFile(const int index, Tensor<float>*& pLabel);
    void readTestLabelFile(const int index, Tensor<float>*& pLabel);

    void oneHotEncodeLabel(const Tensor<float>* pLabel, Tensor<float>*& pOneHotLabel, const int k);
    void saveOneHotCode2LabelFile(Tensor<float>* pOneHotLabel, const string& fullPathFileName, const vector<int>& originalImageTensorSize);

private:
    void readImageFile(const string& filename, Tensor<float>*& pImage);
    void oneHot2Label(Tensor<float>* pOneHotLabel,Tensor<unsigned char>*& pLabel);

    virtual void readLabelFile(const string& filename, Tensor<float>*& pLabel)=0;
    virtual void saveLabel2File(Tensor<unsigned char>* pLabel, const vector<int>& offset, const string& fullPathFileName) = 0;
    virtual void freeItkImageIO() = 0;
};


#endif //CDLF_FRAMEWORK_ITKDATAMANAGER_H
