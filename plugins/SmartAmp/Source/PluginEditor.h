/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "myLookAndFeel.h"

//==============================================================================
/**
*/
class WaveNetVaAudioProcessorEditor  : public AudioProcessorEditor,
                                       private Button::Listener,
                                       private Slider::Listener
                                
{
public:
    WaveNetVaAudioProcessorEditor (WaveNetVaAudioProcessor&);
    ~WaveNetVaAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WaveNetVaAudioProcessor& processor;

    // Amp Widgets
    Slider ampPresenceKnob;
    Slider ampCleanBassKnob;
    Slider ampCleanMidKnob;
    Slider ampCleanTrebleKnob;
    Slider ampCleanGainKnob;
    Slider ampLeadBassKnob;
    Slider ampLeadMidKnob;
    Slider ampLeadTrebleKnob;
    Slider ampLeadGainKnob;
    Slider ampMasterKnob;
    ImageButton ampOnButton;
    ImageButton ampCleanLeadButton;
    ImageButton ampLED;

    
    // LookandFeels 
    myLookAndFeel ampSilverKnobLAF;

    Image background;
    int current_background = 1;

    virtual void buttonClicked(Button* button) override;
    virtual void sliderValueChanged(Slider* slider) override;
    void ampOnButtonClicked();
    void ampCleanLeadButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveNetVaAudioProcessorEditor)
};
