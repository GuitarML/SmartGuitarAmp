/*
  ==============================================================================

    Convolution.cpp
    Created: 3 Jan 2019 10:58:34am
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#include "Convolution.h"

Convolution::Convolution(size_t inputChannels, size_t outputChannels, int filterWidth, int dilation) :
    bias(outputChannels),
    outVec(outputChannels),
    pos(0),
    dilation(dilation),
    inputChannels(inputChannels),
    outputChannels(outputChannels),
    filterWidth(filterWidth)
{
    resetFifo();
    resetKernel();
}

void Convolution::resetKernel()
{
    kernel.clear();
    kernel.reserve(filterWidth);
    for (int i = 0; i < filterWidth; ++i)
    {
        Eigen::MatrixXf x(inputChannels, outputChannels);
        x.setZero();
        kernel.push_back(x);
    }
    bias = Eigen::RowVectorXf (outputChannels);
    bias.setZero();
}

void Convolution::resetFifo()
{
    memory.clear();
    memory.reserve(getFilterOrder());
    for (int i = 0; i < getFilterOrder(); ++i)
    {
        Eigen::RowVectorXf x(inputChannels);
        x.setZero();
        memory.push_back(x);
    }
    pos = 0;
}

void Convolution::setParams(size_t newInputChannels, size_t newOutputChannels,
                            int newFilterWidth, int newDilation)
{
    inputChannels = newInputChannels;
    outputChannels = newOutputChannels;
    filterWidth = newFilterWidth;
    dilation = newDilation;
    outVec = Eigen::RowVectorXf (outputChannels);
    resetFifo();
    resetKernel();
}

int Convolution::getFilterOrder() const
{
    return (filterWidth-1)*dilation + 1;
}

void Convolution::process(float* data, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        processSingleSample(data, i , numSamples);
    }
}

void Convolution::processSingleSample(float* data, int i, int numSamples)  
{
    if (memory.size() != getFilterOrder()) 
        resetFifo();  
    auto fifo = memory.begin();   
    for (int ch = 0; ch < inputChannels; ++ch)  
        (*(fifo+pos))[ch] = data[idx(ch, i, numSamples)]; 
    outVec.setZero();  
    std::vector<Eigen::MatrixXf>::iterator it;
    int j = 0;
    for (auto it = kernel.begin(); it != kernel.end(); it++) 
    {
        int readPos = mod((pos - j * dilation), getFilterOrder()); 
        outVec = outVec + *(fifo+readPos) * (*it);  
        j += 1; 
    }
    outVec = outVec + bias;  
    for (int ch = 0; ch < outputChannels; ++ch)  
        data[idx(ch, i, numSamples)] = outVec[ch];  
    pos = mod(pos + 1, getFilterOrder()); 
}


int Convolution::mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

int Convolution::idx(int ch, int i, int numSamples)
{
    return ch * numSamples + i;
}

void Convolution::setWeight(std::vector<float> W, std::string name)
{
    if (name == "W")
        setKernel(W);
    else if (name == "b")
        setBias(W);
}

void Convolution::setKernel(std::vector<float> W)
{
    assert(W.size() == inputChannels*outputChannels*filterWidth);
    size_t i = 0;
    for (size_t k = 0; k < filterWidth; ++k)
        for(size_t row = 0; row < inputChannels; ++row)
            for (size_t col = 0; col < outputChannels; ++col)
            {
                kernel[filterWidth-1-k](row,col) = W[i];
                i += 1;
            }
}

void Convolution::setBias(std::vector<float> W)
{
    assert(W.size() == outputChannels);
    for (size_t i = 0; i < outputChannels; ++i)
        bias(i) = W[i];
}
