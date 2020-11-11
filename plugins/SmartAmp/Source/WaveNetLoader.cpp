/*
  ==============================================================================

    WaveNetLoader.cpp
    Created: 3 Feb 2019 8:55:31pm
    Author:  Eero-Pekka Damskägg

    Modified by keyth72

  ==============================================================================
*/

#include "WaveNetLoader.h"

WaveNetLoader::WaveNetLoader(var jsonFile) 
{
    // Edit this line to point to your binary json file in project resources
    config = JSON::parse(jsonFile);
    
    numChannels = config["residual_channels"];
    inputChannels = config["input_channels"];
    outputChannels = config["output_channels"];
    filterWidth = config["filter_width"];
    activation = config["activation"].toString().toStdString();
    dilations = readDilations();
}

WaveNetLoader::WaveNetLoader(var jsonFile, File configFile)
{
    // Edit this line to point to your binary json file in project resources
    config = JSON::parse(configFile);

    numChannels = config["residual_channels"];
    inputChannels = config["input_channels"];
    outputChannels = config["output_channels"];
    filterWidth = config["filter_width"];
    activation = config["activation"].toString().toStdString();
    dilations = readDilations();
}

std::vector<int> WaveNetLoader::readDilations()
{
    std::vector<int> newDilations;
    if (auto dilationsArray = config.getProperty("dilations", var()).getArray())
    {
        for (int dil : *dilationsArray)
            newDilations.push_back(dil);
    }
    return newDilations;
}

void WaveNetLoader::loadVariables(WaveNet &model)
{
    if (auto variablesArray = config.getProperty("variables", var()).getArray())
    {
        for (auto& variable : *variablesArray)
        {
            int layerIdx = variable["layer_idx"];
            std::string name = variable["name"].toString().toStdString();
            std::vector<float> data;
            if (auto dataArray = variable.getProperty("data", var()).getArray())
            {
                for (float value : *dataArray)
                    data.push_back(value);
            }
            model.setWeight(data, layerIdx, name);
        }
    }
}
