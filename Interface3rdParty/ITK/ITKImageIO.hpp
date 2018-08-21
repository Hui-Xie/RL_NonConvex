//
// Created by Hui Xie on 8/17/2018.
// Copyright (c) 2018 Hui Xie. All rights reserved.
//

#include "ITKImageIO.h"
#include <vector>
#include "itkImageRegionConstIterator.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

template <typename VoxelType, int Dimension>
ITKImageIO<VoxelType, Dimension>::ITKImageIO(){

}

template <typename VoxelType, int Dimension>
ITKImageIO<VoxelType, Dimension>::~ITKImageIO(){

}

template <typename VoxelType, int Dimension>
void ITKImageIO<VoxelType, Dimension>::readFile(const string& filename, Tensor<float>*& pTensor ){
    using ReaderType = itk::ImageFileReader< ImageType >;
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( filename );
    reader->Update();
    typename ImageType::Pointer image = reader->GetOutput();

    // get ImageSize
    typename ImageType::RegionType region = image->GetLargestPossibleRegion();
    m_imageSize =region.GetSize();
    const int dim = m_imageSize.GetSizeDimension();
    vector<long> tensorSize;
    for (int i=0; i<dim; ++i){
        tensorSize.push_back(m_imageSize[i]);
    }
    pTensor = new Tensor<float>(tensorSize);

    //get Image origin, spacing etc
    m_origin = image->GetOrigin();
    m_spacing = image->GetSpacing();
    m_direction = image->GetDirection();

    itk::ImageRegionConstIterator<ImageType> iter(image,region);
    long i=0;
    iter.GoToBegin();
    while(!iter.IsAtEnd())
    {
        pTensor->e(i)= iter.Get();
        ++iter;
        ++i;
    }

}


template <typename VoxelType, int Dimension>
void ITKImageIO<VoxelType, Dimension>::writeFile(const Tensor<float>* pTensor, const vector<long>& offset,
                                                  const string& filename)
{
    vector<long> tensorSize = pTensor->getDims();
    const int dim = tensorSize.size();
    if (dim != m_imageSize.GetSizeDimension()){
        cout<<"Error: the output tensor has different dimension with input image."<<endl;
        return;
    }

    typename ImageType::RegionType region;
    typename ImageType::IndexType start;
    typename ImageType::SizeType size;
    typename ImageType::PointType newOrigin;
    for(int i=0; i< dim; ++i){
        start[i] = 0;
        size[i] = tensorSize[i];
        newOrigin[i] = m_origin[i]+offset[i]*m_spacing[i];
    }
    region.SetSize(size);
    region.SetIndex(start);

    typename ImageType::Pointer image = ImageType::New();
    image->SetRegions(region);
    image->Allocate();

    //Todo: set origin, spacing, cosine matrix
    image->SetSpacing( m_spacing );
    image->SetOrigin(newOrigin);
    image->SetDirection(m_direction);

    //copy image data
    itk::ImageRegionIterator<ImageType> iter(image,region);
    long i=0;
    iter.GoToBegin();
    while(!iter.IsAtEnd())
    {
        iter.Set(pTensor->e(i));
        ++iter;
        ++i;
    }

    typedef itk::ImageFileWriter<ImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(filename);
    writer->SetInput(image);
    writer->Update();
    cout<<"Info: A output image "<<filename<<" is output"<<endl;
}