//
// Created by Hui Xie on 6/5/2018.
// Copyright (c) 2019 Hui Xie. All rights reserved.

#include "Layer.h"
#include <iostream>
#include <Layer.h>

#include "Tools.h"
using namespace std;

Layer::Layer(const int id, const string& name, const vector<int>& tensorSize){
    //  layerID in net must be greater than 0, and  0 reserves for null layer
    m_id = id;
    m_name = eraseAllSpaces(name);
    m_type = "Layer";
    m_prevLayer = nullptr;
    m_attribute = "";

    m_tensorSize = tensorSize;
    allocateYdYTensor();
}


Layer::~Layer(){
    freeYdYTensor();
}

void Layer::allocateYdYTensor(){
    if (0 != m_tensorSize.size()){
        m_pYTensor = new Tensor<float>(m_tensorSize);
        m_pdYTensor = new Tensor<float>(m_tensorSize);
    }
    else{
        m_pYTensor = nullptr;
        m_pdYTensor = nullptr;
    }
}

void Layer::freeYdYTensor(){
    if (nullptr != m_pYTensor){
        delete m_pYTensor;
        m_pYTensor = nullptr;
    }
    if (nullptr != m_pdYTensor){
        delete m_pdYTensor;
        m_pdYTensor = nullptr;
    }
}

void Layer::zeroYTensor(){
    if ("InputLayer" != m_type && nullptr != m_pYTensor){
        m_pYTensor->zeroInitialize();
    }
}

void Layer::zeroDYTensor(){
    if (nullptr != m_pdYTensor){
        m_pdYTensor->zeroInitialize();
    }
}


void Layer::setAttribute(const string& attr){
    m_attribute = attr;
}

string Layer::getAttribute(){
   return m_attribute;
}


void Layer::printY(){
    cout<<"LayerType: "<<m_type <<"; LayerTensorSize: "<<vector2Str(m_tensorSize)<<"; Y: "<<endl;;
    m_pYTensor->print();
}

void Layer::printDY(){
    cout<<"LayerType: "<<m_type <<"; LayerTensorSize: "<<vector2Str(m_tensorSize)<<"; dY: "<<endl;
    m_pdYTensor->print();
}

/*
void Layer::printVector(Tensor<float>* vector){
    Tensor<float>& Y =   *vector;
    cout<<"( ";
    for (int i=0; i< m_tensorSize; ++i){
        if (i != m_tensorSize-1 ) cout<<Y[i]<<", ";
        else cout<<Y[i];
    }
    cout<<" )"<<endl;
}
*/

void Layer::addPreviousLayer(Layer* prevLayer){
    if (nullptr != prevLayer){
        m_prevLayer = prevLayer;
    }
}

void Layer::allocateOptimizerMem(const string method) {
    //null
}

void Layer::freeOptimizerMem() {
   //null
}
