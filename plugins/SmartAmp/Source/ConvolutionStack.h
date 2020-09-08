/*
  ==============================================================================

    ConvolutionStack.h
    Created: 8 Jan 2019 5:21:49pm
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ConvolutionLayer.h"

class ConvolutionStack
{
public:
    ConvolutionStack(int numChannels, int filterWidth, std::vector<int> dilations,
                     std::string activation, bool residual = true);
    void process(float *data, float* skipData, int numSamples);
    void prepareToPlay(int newNumSamples);
    size_t getNumLayers() { return dilations.size(); }
    void setWeight(std::vector<float> W, int layerIdx, std::string name);
    void setParams(int newNumChannels, int newFilterWidth, std::vector<int> newDilations,
                   std::string newActivation, bool newResidual);
    
private:
    std::vector<ConvolutionLayer> layers;
    std::vector<int> dilations;
    bool residual;
    int numChannels;
    int filterWidth;
    std::string activation;
    int samplesPerBlock = 0;
    AudioBuffer<float> residualData;
    int idx(int ch, int i, int numSamples);
    void copyResidual(float *data, int numSamples);
    void addResidual(float *data, int numSamples);
    float* getSkipPointer(float *skipData, int layerIdx, int numSamples);
    void initLayers();
};
