/*
  ==============================================================================

    WaveNet.h
    Created: 14 Jan 2019 5:19:01pm
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#pragma once

#include <string>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Activations.h"
#include "ConvolutionStack.h"

class WaveNet
{
public:
    WaveNet(int inputChannels, int outputChannels, int convolutionChannels,
            int filterWidth, std::string activation, std::vector<int> dilations);
    void prepareToPlay (int newSamplesPerBlock);
    void process(const float **inputData, float **outputData, int numSamples);
    void setWeight(std::vector<float> W, int layerIdx, std::string name);
    void setParams(int newInputChannels, int newOutputChannels, int newConvChannels,
                   int newFilterWidth, std::string newActivation,
                   std::vector<int> newDilations);
    
private:
    ConvolutionStack convStack;
    ConvolutionLayer inputLayer;
    ConvolutionLayer outputLayer;
    int inputChannels;
    int outputChannels;
    int filterWidth;
    int skipChannels;
    int convolutionChannels;
    int memoryChannels;
    std::string activation;
    std::vector<int> dilations;
    int samplesPerBlock = 0;
    AudioBuffer<float> convData;
    AudioBuffer<float> skipData;
    
    int idx(int ch, int i, int numSamples);
    void readDilations(var config);
    void copyInputData(const float **inputData, int numSamples);
    void copyOutputData(float **outputData, int numSamples);
};
