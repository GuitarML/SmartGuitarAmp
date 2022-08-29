/*
  ==============================================================================

    ConvolutionStack.cpp
    Created: 8 Jan 2019 5:21:49pm
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#include "ConvolutionStack.h"

ConvolutionStack::ConvolutionStack(int numChannels, int filterWidth, std::vector<int> dilations, std::string activation, bool residual) :
    dilations(dilations),
    residual(residual),
    numChannels(numChannels),
    filterWidth(filterWidth),
    activation(activation)
{
    initLayers();
}

void ConvolutionStack::prepareToPlay(int newNumSamples)
{
    samplesPerBlock = newNumSamples;
    residualData.setSize(1, samplesPerBlock * numChannels);
}

void ConvolutionStack::copyResidual(float *data, int numSamples)
{
    auto writePtr = residualData.getWritePointer(0);
    for (size_t i = 0; i < numSamples * numChannels; ++i)
        writePtr[i] = data[i];
}

void ConvolutionStack::addResidual(float *data, int numSamples)
{
    const auto readPtr = residualData.getWritePointer(0);
    for (size_t i = 0; i < numSamples * numChannels; ++i)
        data[i] = data[i] + readPtr[i];
}

void ConvolutionStack::process(float *data, float* skipData, int numSamples)
{
    if (numSamples > samplesPerBlock)
        prepareToPlay(numSamples);
    for (int i = 0; i < dilations.size(); ++i)  
    {
        if (residual)
            copyResidual(data, numSamples);
        float *skipPtr = getSkipPointer(skipData, i, numSamples);
        layers[i].process(data, skipPtr, numSamples);
        if (residual)
            addResidual(data, numSamples);
    }
}

float* ConvolutionStack::getSkipPointer(float *skipData, int layerIdx, int numSamples)
{
    int startCh = numChannels * layerIdx;
    int startIdx = idx(startCh, 0, numSamples);
    return &skipData[startIdx];
}

int ConvolutionStack::idx(int ch, int i, int numSamples)
{
    return ch * numSamples + i;
}

void ConvolutionStack::setWeight(std::vector<float> W, int layerIdx, std::string name)
{
    layers[layerIdx].setWeight(W, name);
}

void ConvolutionStack::setParams(int newNumChannels,
                                 int newFilterWidth,
                                 std::vector<int> newDilations,
                                 std::string newActivation,
                                 bool newResidual)
{
    numChannels = newNumChannels;
    filterWidth = newFilterWidth;
    dilations = newDilations;
    activation = newActivation;
    residual = newResidual;
    initLayers();
    prepareToPlay(samplesPerBlock);
}

void ConvolutionStack::initLayers()
{
    layers.clear();
    layers.reserve(dilations.size());
    for (size_t i = 0; i < dilations.size(); ++i)
        layers.push_back(ConvolutionLayer(numChannels,
                                          numChannels,
                                          filterWidth,
                                          dilations[i],
                                          residual,
                                          activation));
}
