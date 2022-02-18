/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Eq4Band.h"
#include <nlohmann/json.hpp>
#include "RTNeuralLSTM.h"


#define CLEAN_GAIN_ID "cleangain"
#define CLEAN_GAIN_NAME "CleanGain"
#define CLEAN_BASS_ID "cleanbass"
#define CLEAN_BASS_NAME "CleanBass"
#define CLEAN_MID_ID "cleanmid"
#define CLEAN_MID_NAME "CleanMid"
#define CLEAN_TREBLE_ID "cleantreble"
#define CLEAN_TREBLE_NAME "CleanTreble"
#define LEAD_GAIN_ID "leadgain"
#define LEAD_GAIN_NAME "LeadGain"
#define LEAD_BASS_ID "leadbass"
#define LEAD_BASS_NAME "LeadBass"
#define LEAD_MID_ID "leadmid"
#define LEAD_MID_NAME "LeadMid"
#define LEAD_TREBLE_ID "leadtreble"
#define LEAD_TREBLE_NAME "LeadTreble"
#define PRESENCE_ID "presence"
#define PRESENCE_NAME "Presence"
#define MASTER_ID "master"
#define MASTER_NAME "Master"

//==============================================================================
/**
*/
class SmartAmpAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SmartAmpAudioProcessor();
    ~SmartAmpAudioProcessor();

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


    // Overdrive Pedal
    float convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max);

    // Amp
    //void set_ampCleanDrive(float db_ampCleanDrive);
    //void set_ampLeadDrive(float db_ampLeadDrive);
    //void set_ampMaster(float db_ampMaster);
    void set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider);

    float decibelToLinear(float dbValue);

    // Pedal/amp states
    int amp_state = 1; // 0 = off, 1 = on
    int amp_lead = 0; // 0 = clean, 1 = lead
    //int custom_tone = 0; // 0 = custom tone loaded, 1 = default channel tone
    //File loaded_tone;
    //juce::String loaded_tone_name;

    float ampDrive = 0.5;
    //float previousAmpDrive = 0.5;
    //float gainValue = 1.0;
    //float ampMaster = 1.0;
    float previousAmpMaster = 1.0;

    // Amp knob states
    float ampPresenceKnobState = 0.0;
    float ampCleanBassKnobState = 0.0;
    float ampCleanMidKnobState = 0.0;
    float ampCleanTrebleKnobState = 0.0;
    float ampCleanGainKnobState = 0.5;
    float ampLeadBassKnobState = 0.0;
    float ampLeadMidKnobState = 0.0;
    float ampLeadTrebleKnobState = 0.0;
    float ampLeadGainKnobState = 0.5;
    float ampMasterKnobState = 1.0;

    bool lstm_state = true;

    RT_LSTM LSTM;

    AudioProcessorValueTreeState treeState;


private:
    Eq4Band eq4band; // Amp EQ

    // Amp
    //float ampCleanDrive = 1.0;
    //float ampLeadDrive = 1.0;
    //float ampMaster = 1.0;

    var dummyVar;

    chowdsp::ResampledProcess<chowdsp::ResamplingTypes::SRCResampler<>> resampler;

    dsp::IIR::Filter<float> dcBlocker;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmartAmpAudioProcessor)
};
