/*
  ==============================================================================

    ConvolutionLayer.cpp
    Created: 10 Jan 2019 5:04:39pm
    Author:  Damskägg Eero-Pekka

  ==============================================================================
*/

#include "ConvolutionLayer.h"

ConvolutionLayer::ConvolutionLayer(size_t inputChannels,
                                   size_t outputChannels,
                                   int filterWidth,
                                   int dilation,
                                   bool residual,
                                   std::string activationName):
conv(inputChannels,
     Activations::isGated(activationName) ? outputChannels * 2 : outputChannels,
     filterWidth,
     dilation),
out1x1(outputChannels, outputChannels, 1, 1),
residual(residual),
usesGating(Activations::isGated(activationName)),
activation(Activations::getActivationFuncArray(activationName))
{
}

void ConvolutionLayer::process(float* data, int numSamples) 
{
    conv.process(data, numSamples);
    activation(data, conv.getNumOutputChannels(), numSamples);
    if (residual) {
        out1x1.process(data, numSamples);
    }
}

void ConvolutionLayer::process(float* data, float* skipData, int numSamples) 
{
    conv.process(data, numSamples); 
    activation(data, conv.getNumOutputChannels(), numSamples); 
    copySkipData(data, skipData, numSamples); 
    if (residual) {
        out1x1.process(data, numSamples); 
    }
}

void ConvolutionLayer::copySkipData(float *data, float *skipData, int numSamples)
{
    size_t skipChannels = usesGating ? conv.getNumOutputChannels()/2 : conv.getNumOutputChannels();
    for (size_t i = 0; i < (size_t)numSamples*skipChannels; ++i)
        skipData[i] = data[i];
}

void ConvolutionLayer::setParams(size_t newInputChannels, size_t newOutputChannels,
                                 int newFilterWidth, int newDilation, bool newResidual,
                                 std::string newActivationName)
{
    activation = Activations::getActivationFuncArray(newActivationName);
    usesGating = Activations::isGated(newActivationName);
    size_t internalChannels = usesGating ? newOutputChannels * 2 : newOutputChannels;
    conv.setParams(newInputChannels, internalChannels, newFilterWidth, newDilation);
    out1x1.setParams(newOutputChannels, newOutputChannels, 1, 1);
    residual = newResidual;
}

void ConvolutionLayer::setWeight(std::vector<float> W, std::string name)
{
    if ((name == "W_conv") || (name == "W"))
        conv.setWeight(W, "W");
    else if ((name == "b_conv") || (name == "b"))
        conv.setWeight(W, "b");
    else if (name == "W_out")
        out1x1.setWeight(W, "W");
    else if (name == "b_out")
        out1x1.setWeight(W, "b");
}
