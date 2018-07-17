//
// Created by Sheen156 on 6/5/2018.
//

#include "FCLayer.h"
#include "statisTool.h"
#include <iostream>
using namespace std;

// y = W*x+ b
// where y and b is m-D vector, y is output vector;
//       x is n-D input vector
//       W is m*n dimensional matrix
FCLayer::FCLayer(const int id, const string name,const vector<int>& tensorSize, list<Layer*>& preLayers):Layer(id, name, tensorSize){
   m_type = "FullyConnected";
   m_layerParaMap.clear();
   m_m = m_tensorSize[0];

   for(list<Layer*>::const_iterator iter=preLayers.begin(); iter != preLayers.end(); ++iter){
       Layer* preLayer = *iter;
       LayerPara layerPara;
       layerPara.m_n = preLayer->m_tensorSize[0]; //input width
       layerPara.m_pW = new Tensor<float>({m_m,layerPara.m_n});
       layerPara.m_pBTensor =  new Tensor<float>({m_m,1});
       layerPara.m_pdW = new Tensor<float>({m_m,layerPara.m_n});
       layerPara.m_pdBTensor =  new Tensor<float>({m_m,1});
       m_layerParaMap[preLayer] = layerPara;
       addPreviousLayer(preLayer);
   }
}

FCLayer::~FCLayer(){
  for(map<Layer*, LayerPara>::iterator iter = m_layerParaMap.begin(); iter != m_layerParaMap.end(); ++iter){
      LayerPara& layerPara = iter->second;
      if (nullptr != layerPara.m_pW) delete layerPara.m_pW;
      if (nullptr != layerPara.m_pBTensor) delete layerPara.m_pBTensor;

      if (nullptr != layerPara.m_pdW) delete layerPara.m_pdW;
      if (nullptr != layerPara.m_pdBTensor) delete layerPara.m_pdBTensor;
  }
}

void FCLayer::initialize(const string& initialMethod)
{
    for(map<Layer*, LayerPara>::iterator iter = m_layerParaMap.begin(); iter != m_layerParaMap.end(); ++iter) {
        LayerPara &layerPara = iter->second;
        if ("Xavier" == initialMethod) {
            xavierInitialize(layerPara.m_pW);
            long nRow = layerPara.m_pBTensor->getDims()[0];
            const float sigmaB = 1.0 / nRow;
            generateGaussian(layerPara.m_pBTensor, 0, sigmaB);
        }
        else{
            cout<<"Error: Initialize Error in FCLayer."<<endl;
        }
    }

}

void FCLayer::forward(){
    for(list<Layer*>::iterator iter = m_prevLayers.begin(); iter != m_prevLayers.end(); ++iter){
        LayerPara &layerPara = m_layerParaMap[*iter];
        *m_pYTensor += (*layerPara.m_pW) * (*((*iter)->m_pYTensor)) + *(layerPara.m_pBTensor);
    }
}

//   y = W*x +b
//  dL/dW = dL/dy * dy/dW = dL/dy * x'
//  dL/db = dL/dy * dy/db = dL/dy
//  dL/dx = dL/dy * dy/dx = W' * dL/dy
void FCLayer::backward(){
    Tensor<float>& dLdy = *m_pdYTensor;
    *m_pdW = dLdy * trans(*(m_prevLayerPointer->m_pYTensor));
    *m_pdBTensor = dLdy;
    *(m_prevLayerPointer->m_pdYTensor) = trans(*m_pW) * dLdy;
}

void FCLayer::updateParameters(const float lr, const string& method) {
    if ("sgd" == method){
        *m_pW -= lr* (*m_pdW);
        *m_pBTensor -= lr* (*m_pdBTensor);
    }
}

void FCLayer::printWandBVector(){
    cout<<"LayerType: "<<m_type <<"; MatrixSize "<<m_m<<"*"<<m_n<<"; W: "<<endl;
    cout<<*m_pW<<endl;
    cout<<"B-transpose:"<<trans(*m_pBTensor)<<endl;
}

void FCLayer::printdWanddBVector(){
    cout<<"LayerType: "<<m_type <<"; MatrixSize "<<m_m<<"*"<<m_n<<"; dW: "<<endl;
    cout<<*m_pdW<<endl;
    cout<<"dB-transpose:"<<trans(*m_pdBTensor)<<endl;

}
