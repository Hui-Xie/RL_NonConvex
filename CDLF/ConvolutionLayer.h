//
// Created by Hui Xie on 7/19/2018.
//

#ifndef RL_NONCONVEX_CONVOLUTIONLAYER_H
#define RL_NONCONVEX_CONVOLUTIONLAYER_H
#include "Layer.h"
#include <map>

/** Convolution layer
 * Y = W*X
 * where Y is the output at each voxel;
 *       W is the convolution filter, which is uniform in entire input;
 *       X is the receipt region of original input image;
 *       * indicate convolution
 *
 * Notes:
 * 1  in convolution layer, we do not consider bias, as there is a separate BiasLayer for use;
 * 2  Size changes: |Y| = |X|-|W|+1, in their different dimension;
 * 3  it is a good design if all numFilter is odd;
 *
 * */


class ConvolutionLayer :  public Layer {
public:
    ConvolutionLayer(const int id, const string& name, const vector<long>& filterSize, Layer* prevLayer,
                     const int numFilters=1, const int stride=1);
    ~ConvolutionLayer();

    Tensor<float>**  m_pW;
    Tensor<float>**  m_pdW;
    Tensor<float>*  m_expandDy;
    int m_numFilters;
    vector<long> m_filterSize;

    void constructFiltersAndY();


    virtual  void initialize(const string& initialMethod);
    virtual  void zeroParaGradient();
    virtual  void forward();
    virtual  void backward();
    virtual  void updateParameters(const float lr, const string& method, const int batchSize=1);

private:
    int m_stride;
    int m_OneFilterN;

    bool checkFilterSize(const vector<long>& filterSize, Layer* prevLayer);
    void expandDyTensor(const Tensor<float>* pdY);
    void freeExpandDy();
    void computeDW(const Tensor<float>* pdY, Tensor<float>* pdW);
    void computeDX(const Tensor<float>* pdY, const Tensor<float>* pW);//Note: dx need to accumulate along filters
    void updateTensorSize();
    void computeOneFiterN();

};


#endif //RL_NONCONVEX_CONVOLUTIONLAYER_H
