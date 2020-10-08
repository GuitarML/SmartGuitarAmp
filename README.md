# SmartGuitarAmp

Guitar plugin made with JUCE that uses neural network models to emulate real world hardware.

This plugin uses a WaveNet model to recreate the sound of real world hardware. The current version
models a small tube amp, with the ability to add more options in the future. There is a clean/lead channel, 
which is equivalent to the amp's clean and full drive settings. Gain and EQ knobs were added to 
modulate the modeled sound.  

![app](https://github.com/keyth72/SmartGuitarAmp/blob/master/resources/amp_pic.png)

You can create your own models and load them in SmartGuitarAmp with minor code modifications.
To train your own models, use the following Github repository:

https://github.com/keyth72/PedalNetRT

Model training is done using PyTorch on pre recorded .wav samples. More info in the above repository.
Feel free to create a pull request to add your own trained models to this repository.


Also see compainion plugin, the SmartGuitarPedal.
https://github.com/keyth72/SmartGuitarPedal

## Installing the plugin

1. Download plugin (currently Windows 10 only) [here](https://github.com/keyth72/SmartGuitarAmp/releases)
2. Copy to your DAW's VST directory

## Initial Setup

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Download [Eigen](http://eigen.tuxfamily.org)
   Extract Eigen to a convenient location on your system (will be linked with Projucer)
4. Open SmartGuitarPedal.jucer file with Projucer
5. Add the <full_path_to>/ Eigen folder to "Header Search Paths" in Exporters -> Debug/Release
6. Open and build project in Visual Studio (Windows), Xcode (Mac), or Code::Blocks/Makefile (Linux)

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.

## Using your own trained models
The .json models are baked into the executable when you compile the SmartAmp. Separate models are used for clean and lead channels.
To swap out the default models with your own, do the following steps:

1. Open up the project in Projucer and add your json models as a Resource. Right click "Resources", add existing files, and make sure to check "Binary Resource".

2. Open the Visual Studio solution (or other target IDE). You should see the new .json model files under "Resources" in the solution explorer.

3. Open PluginProcessor.cpp and edit lines 208 or 219 (this determines which model is used on the Lead or Clean channel):
```
 (208)  WaveNetLoader loader2(BinaryData::bluej_fullD_p0153_json);    --> 
                     WaveNetLoader loader2(BinaryData::YOUR_NEW_LEAD_MODEL_json);

 (219)  WaveNetLoader loader2(BinaryData::bluej_clean_p0088_json);   -->
                     WaveNetLoader loader2(BinaryData::YOUR_NEW_CLEAN_MODEL_json);
```                     
Where "YOUR_NEW...MODEL" is filename of your json file. You can edit either one, or both, but keep in mind they are separately trained models.

4. Build SmartAmp in Release mode.



This project builds off the work done here:
https://github.com/damskaggep/WaveNetVA

## License
This project is licensed under the Apache License, Version 2.0 - see the [LICENSE](LICENSE) file for details.
