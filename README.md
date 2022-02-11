# SmartGuitarAmp
[![CI](https://github.com/GuitarML/SmartGuitarAmp/actions/workflows/cmake.yml/badge.svg)](https://github.com/GuitarML/SmartGuitarAmp/actions/workflows/cmake.yml) 
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-brightgreen.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Downloads](https://img.shields.io/github/downloads/GuitarML/SmartGuitarAmp/total)](https://somsubhra.github.io/github-release-stats/?username=GuitarML&repository=SmartGuitarAmp&page=1&per_page=30)

Guitar plugin made with JUCE that uses neural network models to emulate real world hardware.

This plugin uses a LSTM neural network model to recreate the sound of real world hardware. Version 2 emulates
a Fender Blues Jr. on the clean channel and a Blackstar HT40 on the lead channel. The Gain parameter is conditioned to fully replicate the gain knob from the actual amplifier.
A traditional 4-band EQ algorithm is used for Bass, Mid, Treble, and Presence on each channel. 
 
Note: For the previous version of SmartAmp using the WaveNet model, see version 1.3 on the release page.
Version 2 was created using the LSTM model to improve CPU usage and bring SmartAmp up to current GuitarML standards.

![app](https://github.com/keyth72/SmartGuitarAmp/blob/master/resources/amp_pic.png)

## Info
Re-creation of the LSTM inference model from [Real-Time Guitar Amplifier Emulation with Deep
Learning](https://www.mdpi.com/2076-3417/10/3/766/htm)

The [Automated-GuitarAmpModelling](https://github.com/Alec-Wright/Automated-GuitarAmpModelling) project was used to train the .json models.<br>
GuitarML maintains a [fork](https://github.com/GuitarML/Automated-GuitarAmpModelling) with a few extra helpful features, including a Colab training script.

The plugin uses [RTNeural](https://github.com/jatinchowdhury18/RTNeural), which is a highly optimized neural net inference engine intended for audio applications. 

## Installing the plugin

1. Download the appropriate plugin installer (Windows, Mac, Linux)
2. Run the installer and follow the instructions.

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
The binaries will be located in `SmartGuitarAmp/build/SmartAmp_artefacts/`

### Build with Projucer 

1. Clone or download this repository.
2. Download and install [JUCE](https://juce.com/) This project uses the "Projucer" application from the JUCE website. 
3. Initialize and set up submodules
```git submodule update --init --recursive```
4. Open the Chameleon.jucer file and in the appropriate Exporter Header Search Path field, enter the appropriate include paths from the modules folder.
 
5. Build SmartAmp from the Juce Projucer application. 

Note: Make sure to build in Release mode unless actually debugging. Debug mode will not keep up with real time playing.
