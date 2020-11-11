/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveNet.h"
#include "WaveNetLoader.h"
#include "Eq4Band.h"

//==============================================================================
/**
*/
class WaveNetVaAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    WaveNetVaAudioProcessor();
    ~WaveNetVaAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadConfigAmp();
    void loadConfig(File configFile);

    // Overdrive Pedal
    float convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max);

    // Amp
    void set_ampCleanDrive(float db_ampCleanDrive);
    void set_ampLeadDrive(float db_ampLeadDrive);
    void set_ampMaster(float db_ampMaster);
    void set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider);

    float decibelToLinear(float dbValue);

    // Pedal/amp states
    int amp_state = 1; // 0 = off, 1 = on
    int amp_lead = 1; // 1 = clean, 0 = lead
    File loaded_tone;
    juce::String loaded_tone_name;

    // Amp knob states
    float ampPresenceKnobState = 0.0;
    float ampCleanBassKnobState = 0.0;
    float ampCleanMidKnobState = 0.0;
    float ampCleanTrebleKnobState = 0.0;
    float ampCleanGainKnobState = 10.0;
    float ampLeadBassKnobState = 0.0;
    float ampLeadMidKnobState = 0.0;
    float ampLeadTrebleKnobState = 0.0;
    float ampLeadGainKnobState = 10.0;
    float ampMasterKnobState = -12.0;

private:
    WaveNet waveNet; // Amp Clean Channel / Lead Channel

    Eq4Band eq4band; // Amp EQ


    // Amp
    float ampCleanDrive = 1.0;
    float ampLeadDrive = 1.0;
    float ampMaster = 1.0;

    var dummyVar;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveNetVaAudioProcessor)
};
