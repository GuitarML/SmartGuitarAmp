# SmartGuitarAmp

Guitar plugin made with JUCE that uses neural network models to emulate real world hardware.

This plugin uses a WaveNet model to recreate the sound of real world hardware. The current version
models a small tube amp, with the ability to add more options in the future. There is a clean/lead channel, 
which is equivalent to the amp's clean and full drive settings. Gain and EQ knobs were added to 
modulate the modeled sound.  

![app](https://github.com/keyth72/SmartGuitarAmp/blob/master/resources/amp_pic.png)

You can create your own models and load them in SmartGuitarAmp with minor code modifications.
To train your own models, use the following Github repository:

https://github.com/keyth72/pedalnet

Model training is done using PyTorch on pre recorded .wav samples. More info in the above repository.
Feel free to create a pull request to add your own trained models to this repository.


Also see compainion plugin, the SmartGuitarPedal.
https://github.com/keyth72/SmartGuitarPedal


## Initial Setup

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Download [Eigen](http://eigen.tuxfamily.org)
   Extract Eigen to a convenient location on your system (will be linked with Projucer)
4. Open SmartGuitarPedal.jucer file with Projucer
5. Add the <full_path_to>/ Eigen folder to "Header Search Paths" in Exporters -> Debug/Release
6. Open and build project in Visual Studio (Windows), Xcode (Mac), or Code::Blocks/Makefile (Linux)

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.

This project builds off the work done here:
https://github.com/damskaggep/WaveNetVA

## License
This project is licensed under the Apache License, Version 2.0 - see the [LICENSE](LICENSE) file for details.
