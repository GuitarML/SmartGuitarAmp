# SmartGuitarAmp

Guitar plugin made with JUCE that uses neural network models to emulate real world hardware.

See video demo on [YouTube](https://youtu.be/oIX5y-nyhcY)

This plugin uses a WaveNet model to recreate the sound of real world hardware. The current version
models a small tube amp, with the ability to add more options in the future. There is a clean/lead channel, 
which is equivalent to the amp's clean and full drive settings. Gain and EQ knobs were added to 
modulate the modeled sound.

![app](https://github.com/keyth72/SmartGuitarAmp/blob/master/resources/amp_pic.png)

You can create your own models and load them in SmartGuitarAmp with minor code modifications.
To train your own models, use [PedalNetRT](https://github.com/GuitarML/PedalNetRT)

Model training is done using PyTorch on pre recorded .wav samples. More info in the above repository.
To share your best models, email the json files to smartguitarml@gmail.com and they may be included 
in the latest release as a downloadable zip.

Also see compainion plugin, the [SmartGuitarPedal](https://github.com/GuitarML/SmartGuitarPedal)

## Installing the plugin

1. Download plugin (Windows 10, Mac, Ubuntu Linux) [here](https://github.com/keyth72/SmartGuitarAmp/releases)
2. Copy to your DAW's VST directory (for Mac, use .dmg installer or copy AU/VST3 to desired folder)

## Build Instructions

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Download [Eigen](http://eigen.tuxfamily.org)
   Extract Eigen to a convenient location on your system (will be linked with Projucer)
4. Open SmartGuitarPedal.jucer file with Projucer
5. Add the <full_path_to>/ Eigen folder to "Header Search Paths" in Exporters -> Debug/Release
6. Open and build project in Visual Studio (Windows), Xcode (Mac), or Code::Blocks/Makefile (Linux)

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.

## Using your own custom trained models (or models from the TonePack)

Use the "Load Tone" button in the plugin to load tone models trained with PedalNetRT.  The current channel's 
EQ/gain will be applied to the custom tone.  Switching the clean/lead channel unloads the custom tone and 
reloads the channel's default tone.

## License
This project is licensed under the Apache License, Version 2.0 - see the [LICENSE](LICENSE) file for details.

This project builds off the work done in [WaveNetVA](https://github.com/damskaggep/WaveNetVA)

The EQ code used in this plugin is based on the work done by Michael Gruhn in 4BandEQ algorithm.
