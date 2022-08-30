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
    void set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider);


    // Pedal/amp states
    int amp_state = 1; // 0 = off, 1 = on
    int amp_lead = 1; // 1 = clean, 0 = lead
    int custom_tone = 0; // 0 = custom tone loaded, 1 = default channel tone
    File loaded_tone;
    juce::String loaded_tone_name;

    AudioProcessorValueTreeState treeState;

private:
    WaveNet waveNet; // Amp Clean Channel / Lead Channel
    Eq4Band eq4band; // Amp EQ

    std::atomic<float>* presenceParam = nullptr;
    std::atomic<float>* cleanBassParam = nullptr;
    std::atomic<float>* cleanMidParam = nullptr;
    std::atomic<float>* cleanTrebleParam = nullptr;
    std::atomic<float>* cleanGainParam = nullptr;
    std::atomic<float>* leadGainParam = nullptr;
    std::atomic<float>* leadBassParam = nullptr;
    std::atomic<float>* leadMidParam = nullptr;
    std::atomic<float>* leadTrebleParam = nullptr;
    std::atomic<float>* masterParam = nullptr;


    float previousGainValue = 0.5;
    float previousMasterValue = 0.5;

    var dummyVar;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveNetVaAudioProcessor)
};
