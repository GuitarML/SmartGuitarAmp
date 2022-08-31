/*
  ==============================================================================

    ConvolutionLayer.h
    Created: 10 Jan 2019 5:04:39pm
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#pragma once

#include <string>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Convolution.h"
#include "Activations.h"

class ConvolutionLayer
{
public:
    ConvolutionLayer(size_t inputChannels,
                     size_t outputChannels,
                     int filterWidth,
                     int dilation = 1,
                     bool residual = false,
                     std::string activationName = "linear");
    void process(float* data, int numSamples);
    void process(float* data, float* skipdata, int numSamples);
    void setParams(size_t newInputChannels, size_t newOutputChannels, int newFilterWidth,
                   int newDilation, bool newResidual, std::string newActivationName);
    void setWeight(std::vector<float> W, std::string name);
    
private:
    Convolution conv;
    Convolution out1x1;
    bool residual;
    bool usesGating;
    typedef void (* activationFunction)(float *x , size_t rows, size_t cols);
    activationFunction activation;
    void copySkipData(float *data, float *skipData, int numSamples);
};
