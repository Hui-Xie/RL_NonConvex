//
// Created by Hui Xie on 8/6/2018.
//

#include "MnistConvNet.h"
#include "../../Interfaces3ndParty/MNIST/MNIST.h"

const string mnistDir= "E:\\CProject\\mnist";

int main (int argc, char *argv[])
{
    MNIST mnist(mnistDir);
    mnist.loadData();

    cout<<"test the matching between image and label in whole dataset"<<endl;
    srand (time(NULL));
    long index = rand() % 10000;
    mnist.displayImage(mnist.m_pTestImages, index);
    cout<<"Image is "<<(int)(mnist.m_pTestLabels->e(index))<<endl;

    //tailor data and delete wholeDataSet, and keep PartDataSet
    mnist.tailorData();
    mnist.deleteWholeDataSet();

    cout<<"test the matching between image and label in part dataset"<<endl;
    index = rand() % 10000;
    mnist.displayImage(mnist.m_pTrainImagesPart, index);
    cout<<"Image is "<<(int)(mnist.m_pTrainLabelsPart->e(index))<<endl;

    MnistConvNet net(&mnist);
    net.build();
    net.setNetParameters();
    net.printArchitecture();
    long epoch= 12;
    float accuracy = 0;
    for (long i=0; i<epoch; ++i){
        net.train();
        accuracy = net.test();
        cout<<"Epoch_"<<i<<": "<<" accuracy = "<<accuracy<<endl;
     }
    cout<<"==========End of Mnist Test==========="<<endl;
    return 0;
}