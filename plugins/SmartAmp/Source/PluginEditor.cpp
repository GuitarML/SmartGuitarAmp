/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveNetVaAudioProcessorEditor::WaveNetVaAudioProcessorEditor (WaveNetVaAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

    // Set Widget Graphics
    ampSilverKnobLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::knob_silver_png, BinaryData::knob_silver_pngSize));

    ampOnButton.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    addAndMakeVisible(ampOnButton);
    ampOnButton.addListener(this);

    ampCleanLeadButton.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    addAndMakeVisible(ampCleanLeadButton);
    ampCleanLeadButton.addListener(this);

    ampLED.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    addAndMakeVisible(ampLED);

    addAndMakeVisible(ampPresenceKnob);
    ampPresenceKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampPresenceKnob.addListener(this);
    //ampPresenceKnob.setSkewFactorFromMidPoint(1000.0); // Not working because of custom lookAndFeel class
    ampPresenceKnob.setRange(-10.0, 10.0);
    ampPresenceKnob.setValue(processor.ampPresenceKnobState);
    ampPresenceKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampPresenceKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 75, 20);
    ampPresenceKnob.setNumDecimalPlacesToDisplay(1);
    ampPresenceKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampCleanBassKnob);
    ampCleanBassKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampCleanBassKnob.addListener(this);
    ampCleanBassKnob.setRange(-8.0, 8.0);
    ampCleanBassKnob.setValue(processor.ampCleanBassKnobState);
    ampCleanBassKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampCleanBassKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampCleanBassKnob.setNumDecimalPlacesToDisplay(1);
    ampCleanBassKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampCleanMidKnob);
    ampCleanMidKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampCleanMidKnob.addListener(this);
    ampCleanMidKnob.setRange(-8.0, 8.0);
    ampCleanMidKnob.setValue(processor.ampCleanMidKnobState);
    ampCleanMidKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampCleanMidKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampCleanMidKnob.setNumDecimalPlacesToDisplay(1);
    ampCleanMidKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampCleanTrebleKnob);
    ampCleanTrebleKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampCleanTrebleKnob.addListener(this);
    ampCleanTrebleKnob.setRange(-8.0, 8.0);
    ampCleanTrebleKnob.setValue(processor.ampCleanTrebleKnobState);
    ampCleanTrebleKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampCleanTrebleKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampCleanTrebleKnob.setNumDecimalPlacesToDisplay(1);
    ampCleanTrebleKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampCleanGainKnob);
    ampCleanGainKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampCleanGainKnob.addListener(this);
    ampCleanGainKnob.setRange(0.0, 20.0);
    ampCleanGainKnob.setValue(processor.ampCleanGainKnobState);
    ampCleanGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampCleanGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampCleanGainKnob.setNumDecimalPlacesToDisplay(1);
    ampCleanGainKnob.setDoubleClickReturnValue(true, 10.0);

    addAndMakeVisible(ampLeadBassKnob);
    ampLeadBassKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampLeadBassKnob.addListener(this);
    ampLeadBassKnob.setRange(-8.0, 8.0);
    ampLeadBassKnob.setValue(processor.ampLeadBassKnobState);
    ampLeadBassKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampLeadBassKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampLeadBassKnob.setNumDecimalPlacesToDisplay(1);
    ampLeadBassKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampLeadMidKnob);
    ampLeadMidKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampLeadMidKnob.addListener(this);
    ampLeadMidKnob.setRange(-8.0, 8.0);
    ampLeadMidKnob.setValue(processor.ampLeadMidKnobState);
    ampLeadMidKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampLeadMidKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampLeadMidKnob.setNumDecimalPlacesToDisplay(1);
    ampLeadMidKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampLeadTrebleKnob);
    ampLeadTrebleKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampLeadTrebleKnob.addListener(this);
    ampLeadTrebleKnob.setRange(-8.0, 8.0);
    ampLeadTrebleKnob.setValue(processor.ampLeadTrebleKnobState);
    ampLeadTrebleKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampLeadTrebleKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampLeadTrebleKnob.setNumDecimalPlacesToDisplay(1);
    ampLeadTrebleKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampLeadGainKnob);
    ampLeadGainKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampLeadGainKnob.addListener(this);
    ampLeadGainKnob.setRange(0.0, 20.0);
    ampLeadGainKnob.setValue(processor.ampLeadGainKnobState);
    ampLeadGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampLeadGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampLeadGainKnob.setNumDecimalPlacesToDisplay(1);
    ampLeadGainKnob.setDoubleClickReturnValue(true, 10.0);

    addAndMakeVisible(ampMasterKnob);
    ampMasterKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setRange(-24.0, 0.0);
    ampMasterKnob.setValue(processor.ampMasterKnobState);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.setDoubleClickReturnValue(true, -12.0);

    // Size of plugin GUI
    setSize (1085, 660);

    // Load the preset wavenet json model from the project resources
    processor.loadConfigAmp();
}

WaveNetVaAudioProcessorEditor::~WaveNetVaAudioProcessorEditor()
{
}

//==============================================================================
void WaveNetVaAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    if ( current_background == 1 && processor.amp_state == 1 && processor.amp_lead == 1 ) {
        background = ImageCache::getFromMemory(BinaryData::amp_clean_png, BinaryData::amp_clean_pngSize);
    } else if (current_background == 1 && processor.amp_state == 1 && processor.amp_lead == 0) {
        background = ImageCache::getFromMemory(BinaryData::amp_lead_png, BinaryData::amp_lead_pngSize);
    } else {
        background = ImageCache::getFromMemory(BinaryData::amp_off_png, BinaryData::amp_off_pngSize);
    }
    g.drawImageAt(background, 0, 0);

    g.setColour (Colours::white);
    g.setFont (15.0f);

    // Should really override the ToggleButton class, but being lazy here
    // Set On/Off amp graphic
    if (processor.amp_state == 0) {
        ampOnButton.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
            0.0);
        ampLED.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::led_blue_off_png, BinaryData::led_blue_off_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::led_blue_off_png, BinaryData::led_blue_off_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    else {
        ampOnButton.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
            0.0);
        ampLED.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    // Set clean/lead switch graphic
    if (processor.amp_lead == 0) {
        ampCleanLeadButton.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    else {
        ampCleanLeadButton.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
}

void WaveNetVaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Amp Widgets
    ampPresenceKnob.setBounds(97, 495, 75, 105);
    ampCleanBassKnob.setBounds(197, 495, 75, 105);
    ampCleanMidKnob.setBounds(280, 495, 75, 105);
    ampCleanTrebleKnob.setBounds(378, 495, 75, 105);
    ampCleanGainKnob.setBounds(456, 495, 75, 105);
    ampLeadBassKnob.setBounds(553, 495, 75, 105);
    ampLeadMidKnob.setBounds(636, 495, 75, 105);
    ampLeadTrebleKnob.setBounds(726, 495, 75, 105);
    ampLeadGainKnob.setBounds(806, 495, 75, 105);
    ampMasterKnob.setBounds(903, 495, 75, 105);

    ampOnButton.setBounds(9, 495, 35, 45);
    ampCleanLeadButton.setBounds(959, 495, 15, 25);

    ampLED.setBounds(975, 160, 15, 25);
}

void WaveNetVaAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &ampOnButton)
        ampOnButtonClicked();
    else if (button == &ampCleanLeadButton)
        ampCleanLeadButtonClicked();
}


void WaveNetVaAudioProcessorEditor::ampOnButtonClicked() {
    if (processor.amp_state == 0) {
        processor.amp_state = 1;
    }
    else {
        processor.amp_state = 0;
    }
    repaint();
}

void WaveNetVaAudioProcessorEditor::ampCleanLeadButtonClicked() {
    if (processor.amp_lead == 0) {
        processor.amp_lead = 1;
        processor.loadConfigAmp();
        processor.set_ampEQ(ampCleanBassKnob.getValue(), ampCleanMidKnob.getValue(), ampCleanTrebleKnob.getValue(), ampPresenceKnob.getValue());
    }
    else if (processor.amp_lead == 1) {
        processor.amp_lead = 0;
        processor.loadConfigAmp();
        processor.set_ampEQ(ampLeadBassKnob.getValue(), ampLeadMidKnob.getValue(), ampLeadTrebleKnob.getValue(), ampPresenceKnob.getValue());
    }
    repaint();
}

void WaveNetVaAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    // Amp
    if (slider == &ampCleanGainKnob)
        processor.set_ampCleanDrive(slider->getValue());
    else if (slider == &ampLeadGainKnob)
        processor.set_ampLeadDrive(slider->getValue());
    else if (slider == &ampMasterKnob)
        processor.set_ampMaster(slider->getValue());
    else if (slider == &ampCleanBassKnob || slider == &ampCleanMidKnob || slider == &ampCleanTrebleKnob) {
        if (processor.amp_lead == 1)
            processor.set_ampEQ(ampCleanBassKnob.getValue(), ampCleanMidKnob.getValue(), ampCleanTrebleKnob.getValue(), ampPresenceKnob.getValue());
    }
    else if (slider == &ampLeadBassKnob || slider == &ampLeadMidKnob || slider == &ampLeadTrebleKnob) {
        if (processor.amp_lead == 0)
            processor.set_ampEQ(ampLeadBassKnob.getValue(), ampLeadMidKnob.getValue(), ampLeadTrebleKnob.getValue(), ampPresenceKnob.getValue());
    }
    else if (slider == &ampPresenceKnob) {
        if (processor.amp_lead == 1)
            processor.set_ampEQ(ampCleanBassKnob.getValue(), ampCleanMidKnob.getValue(), ampCleanTrebleKnob.getValue(), ampPresenceKnob.getValue());
        else if (processor.amp_lead == 0)
            processor.set_ampEQ(ampLeadBassKnob.getValue(), ampLeadMidKnob.getValue(), ampLeadTrebleKnob.getValue(), ampPresenceKnob.getValue());
    }

}

