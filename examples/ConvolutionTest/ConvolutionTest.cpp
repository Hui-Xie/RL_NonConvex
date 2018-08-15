//
// Created by Hui Xie on 8/13/2018.
//
#include "CDLF.h"

/*  inputLayer: 5*5
 *  ConvLayer: 1 filter of size 3*3;
 *  VectorizationLayer: 9*1
 *  LossConvexExmaple1: 1
 *
 *  * */

#include "LossConvexExample1.h"

int main (int argc, char *argv[])
{
    Net net;

    // build network
    int id =1;
    InputLayer* inputLayer = new InputLayer(id++, "InputLayer", {5,5});
    net.addLayer(inputLayer);

    ConvolutionLayer* conv1 = new ConvolutionLayer(id++, "Conv1", {3,3}, net.getFinalLayer());
    net.addLayer(conv1);

    VectorizationLayer* vec1 = new VectorizationLayer(id++, "Vec1", net.getFinalLayer());
    net.addLayer(vec1);

    LossConvexExample1* loss = new LossConvexExample1(id++, "Loss", net.getFinalLayer());
    net.addLayer(loss);

    // config network parameters;
    net.setLearningRate(0.01);
    net.setLossTolerance(0.02);
    net.setMaxIteration(100);
    net.setBatchSize(1);

    net.printArchitecture();

    //  run network
    net.initialize();

    map<int,Layer*> netLayers = net.getLayersMap();

    Tensor<float> inputTensor({5,5});
    //inputTensor.uniformIntialize(1);
    inputLayer->setInputTensor(inputTensor);
    long i=0;
    while (i< 100){
        generateGaussian(&inputTensor,0,1);
        inputLayer->setInputTensor(inputTensor);
        net.zeroParaGradient();
        net.forwardPropagate();

        //for test
        for(map<int, Layer*>::iterator iter = netLayers.begin(); iter != netLayers.end(); ++iter){
            cout<<iter->second->m_name<<": Y"<<endl;
            iter->second->m_pYTensor->printElements();
        }

        net.printIteration(loss, i);
        net.backwardPropagate();

        //for test
        for (map<int, Layer*>::reverse_iterator rit=netLayers.rbegin(); rit!=netLayers.rend(); ++rit){
             if ("ConvolutionLayer" == rit->second->m_type){
                cout<<"dy: "<<endl;
                rit->second->m_pdYTensor->printElements();
                cout<<"dw:"<<endl;
                ((ConvolutionLayer*)rit->second)->m_pdW[0]->printElements();
             }
        }
        
        net.sgd(net.getLearningRate(), 1);
        ++i;
    }
    loss->printGroundTruth();

    cout<< "End of ConvolutionLayer Test."<<endl;
    return 0;
}