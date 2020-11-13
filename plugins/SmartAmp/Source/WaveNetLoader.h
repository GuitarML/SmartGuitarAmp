/*
  ==============================================================================

    WaveNetLoader.h
    Created: 3 Feb 2019 8:55:31pm
    Author:  Eero-Pekka Damskägg

    Modified by keyth72

  ==============================================================================
*/

#pragma once
#include <string>

#include "../JuceLibraryCode/JuceHeader.h"

#include "WaveNet.h"

class WaveNetLoader
{
public:
    WaveNetLoader(var jsonFile);
    WaveNetLoader(var jsonFile, File configFile);
    int numChannels;
    int inputChannels;
    int outputChannels;
    int filterWidth;
    std::vector<int> dilations;
    std::string activation;
    void loadVariables(WaveNet &model);
    var current_jsonFile;
private:
    std::vector<int> readDilations();
    var config;

};
