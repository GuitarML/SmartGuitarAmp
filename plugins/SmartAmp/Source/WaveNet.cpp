/*
  ==============================================================================

    WaveNet.cpp
    Created: 14 Jan 2019 5:19:01pm
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#include "WaveNet.h"


WaveNet::WaveNet(int inputChannels, int outputChannels, int convolutionChannels,
                 int filterWidth, std::string activation, std::vector<int> dilations) :
    convStack(convolutionChannels, filterWidth, dilations, activation),
    inputLayer(inputChannels, convolutionChannels, 1),
    outputLayer(convolutionChannels*dilations.size(), outputChannels, 1),
    inputChannels(inputChannels),
    outputChannels(outputChannels),
    filterWidth(filterWidth),
    skipChannels(convolutionChannels*(int)dilations.size()),
    convolutionChannels(convolutionChannels),
    memoryChannels(Activations::isGated(activation) ? convolutionChannels * 2 : convolutionChannels),
    activation(activation),
    dilations(dilations)
{
}

void WaveNet::readDilations(var config)
{
    std::vector<int> newDilations;
    if (auto dilationsArray = config.getProperty("dilations", var()).getArray())
    {
        for (int dil : *dilationsArray)
            newDilations.push_back(dil);
    }
    dilations = newDilations;
}


void WaveNet::prepareToPlay(int newSamplesPerBlock)
{
    samplesPerBlock = newSamplesPerBlock;
    convData.setSize(1, samplesPerBlock*memoryChannels);
    skipData.setSize(1, samplesPerBlock*skipChannels);
    convStack.prepareToPlay(samplesPerBlock);
}

void WaveNet::copyInputData(const float **inputData, int numSamples)
{
    float *writePtr = convData.getWritePointer(0);
    for (int ch = 0; ch < inputChannels; ++ch)
    {
        int start_idx = idx(ch, 0, numSamples);
        const float *chData = inputData[ch];
        for (int i = 0; i < numSamples; ++i)
            writePtr[start_idx + i] = chData[i];
    }
}

void WaveNet::copyOutputData(float **outputData, int numSamples)
{
    const float *readPtr = skipData.getReadPointer(0);
    for (int ch = 0; ch < outputChannels; ++ch)
    {
        int start_idx = idx(ch, 0, numSamples);
        float *chData = outputData[ch];
        for (int i = 0; i < numSamples; ++i)
            chData[i] = readPtr[start_idx + i];
    }
}

void WaveNet::process(const float **inputData, float **outputData, int numSamples) 
{
    if (numSamples > samplesPerBlock)
        prepareToPlay(numSamples);
    copyInputData(inputData, numSamples);  
    inputLayer.process(convData.getWritePointer(0), numSamples);
    convStack.process(convData.getWritePointer(0), skipData.getWritePointer(0), numSamples);
    outputLayer.process(skipData.getWritePointer(0), numSamples);
    copyOutputData(outputData, numSamples);
}

int WaveNet::idx(int ch, int i, int numSamples)
{
    return ch * numSamples + i;
}

void WaveNet::setWeight(std::vector<float> W, int layerIdx, std::string name)
{
    if (layerIdx < 0)
    {
        inputLayer.setWeight(W, name);
    }
    else if (layerIdx >= convStack.getNumLayers())
    {
        outputLayer.setWeight(W, name);
    }
    else
    {
        convStack.setWeight(W, layerIdx, name);
    }
}

void WaveNet::setParams(int newInputChannels, int newOutputChannels, int newConvChannels,
                        int newFilterWidth, std::string newActivation,
                        std::vector<int> newDilations)
{
    inputChannels = newInputChannels;
    outputChannels = newOutputChannels;
    activation = newActivation;
    convolutionChannels = newConvChannels;
    memoryChannels = Activations::isGated(activation) ? convolutionChannels * 2 : convolutionChannels;
    filterWidth = newFilterWidth;
    dilations = newDilations;
    skipChannels = convolutionChannels * (int)dilations.size();
    inputLayer.setParams(inputChannels, convolutionChannels, 1, 1, false, "linear");
    outputLayer.setParams(skipChannels, outputChannels, 1, 1, false, "linear");
    convStack.setParams(convolutionChannels, filterWidth, dilations, activation, true);
    prepareToPlay(samplesPerBlock);
}
