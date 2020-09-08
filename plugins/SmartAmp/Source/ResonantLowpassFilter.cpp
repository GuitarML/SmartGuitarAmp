/*
  ==============================================================================

  ResonantLowpassFilter
  
  ==============================================================================
*/

#include "ResonantLowpassFilter.h"

ResonantLowpassFilter::ResonantLowpassFilter()
{
    setParameters(10000.0, 44100);
}

void ResonantLowpassFilter::process (AudioBuffer<float>& buffer, 
                                    MidiBuffer& midiMessages, 
                                    const int numSamples, 
                                    const int numInputChannels)
{
    for (int channel = 0; channel < numInputChannels; ++channel) {
        float* channelData = buffer.getWritePointer(channel);
        // For each sample in the block of audio, apply filter
        for (int sample = 0; sample < numSamples; ++sample) {
            const float in = channelData[sample];
            n3 = n3 + cut_lp * (in - n3 + fb_lp * (n3 - n4));
            n4 = n4 + cut_lp * (n3 - n4);
            // Write output to audio buffer
            channelData[sample] = n4;
        }
    }
}

void ResonantLowpassFilter::setParameters(float toneKnobValue, float sampleRate)
{
    toneFrequency = toneKnobValue;
    cut_lp = toneFrequency * 2 / sampleRate;  
    fb_lp = res_lp / (1 - cut_lp);
    n3 = 0;
    n4 = 0;
}