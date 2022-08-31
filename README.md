# SmartGuitarAmp

[![Downloads](https://img.shields.io/github/downloads/GuitarML/SmartGuitarAmp/total)](https://somsubhra.github.io/github-release-stats/?username=GuitarML&repository=SmartGuitarAmp&page=1&per_page=30)

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

Also see companion plugin, the [SmartGuitarPedal](https://github.com/GuitarML/SmartGuitarPedal)<br>
Note: As of SmartAmp version 1.3, the custom model load was removed to simplify the plugin. To load user
trained models, use the SmartGuitarPedal, which plays all models trained with PedalNetRT.

## Installing the plugin

1. Download the appropriate plugin installer (Windows, Mac, Linux)
2. Run the installer and follow the instructions. May need to reboot to allow your DAW to recognize the new plugin.

## Build Instructions

### Build with Cmake

```bash
# Clone the repository
$ git clone https://github.com/GuitarML/SmartGuitarAmp.git
$ cd SmartGuitarAmp

# initialize and set up submodules
$ git submodule update --init --recursive

# build with CMake
$ cmake -Bbuild
$ cmake --build build --config Release
```
The binaries will be located in `SmartAmp/build/SmartAmp_artefacts/`

## License
This project is licensed under the Apache License, Version 2.0 - see the [LICENSE](LICENSE) file for details.

This project builds off the work done in [WaveNetVA](https://github.com/damskaggep/WaveNetVA)

The EQ code used in this plugin is based on the work done by Michael Gruhn in 4BandEQ algorithm.
