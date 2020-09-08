/*
  ==============================================================================

  ResonantLowpassFilter

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================

class ResonantLowpassFilter
{
public:
    ResonantLowpassFilter();
    void process (AudioBuffer<float>& buffer, MidiBuffer& midiMessages, const int numSamples, const int numInputChannels);
    void setParameters(float toneKnobValue, float sampleRate);

private:
    // Tone Knob related variables
    float sampleRate = 44100; //TODO set from var
    float toneFrequency = 9600;
    float cut_lp = toneFrequency * 2 / sampleRate; 
    float res_lp = 0.7; // Resonance strength (range 0 to 1)
    float fb_lp = res_lp / (1 - cut_lp);
    float n3 = 0.0; 
    float n4 = 0.0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonantLowpassFilter)
};
