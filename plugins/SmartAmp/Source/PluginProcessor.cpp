/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveNetVaAudioProcessor::WaveNetVaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    waveNet(1, 1, 1, 1, "linear", { 1 })
    
#endif
{
}

WaveNetVaAudioProcessor::~WaveNetVaAudioProcessor()
{
}

//==============================================================================
const String WaveNetVaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveNetVaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveNetVaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveNetVaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveNetVaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveNetVaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveNetVaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveNetVaAudioProcessor::setCurrentProgram (int index)
{
}

const String WaveNetVaAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveNetVaAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WaveNetVaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    waveNet.prepareToPlay(samplesPerBlock);
}

void WaveNetVaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveNetVaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaveNetVaAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    // Setup Audio Data
    const int numSamples = buffer.getNumSamples();
    const int numInputChannels = getTotalNumInputChannels();


    // Amp =============================================================================
    if (amp_state == 1) {
        //    EQ TODO before or after wavenet? (Presence; Bass, Mid, Treble for both channels)
        eq4band.process(buffer, midiMessages, numSamples, numInputChannels);

        if (amp_lead == 1) {// if clean channel eq/gain
            
            buffer.applyGain(ampCleanDrive);

        }
        else {// else lead channel eq/gain

            buffer.applyGain(ampLeadDrive);

        }

        //    Wavenet, load json for waveNet2 based on lead/clean switch
        waveNet.process(buffer.getArrayOfReadPointers(), buffer.getArrayOfWritePointers(), buffer.getNumSamples());

        //    Master Volume 
        buffer.applyGain(ampMaster);

        //    Apply levelAdjust from model param (for adjusting quiet or loud models)
        if ( waveNet.levelAdjust != 0.0 ) {
            buffer.applyGain(waveNet.levelAdjust);
        }

    }
    
    for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());
}

//==============================================================================
bool WaveNetVaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WaveNetVaAudioProcessor::createEditor()
{
    return new WaveNetVaAudioProcessorEditor (*this);
}

//==============================================================================
void WaveNetVaAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaveNetVaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


void WaveNetVaAudioProcessor::loadConfigAmp() 
{
    // Load Second Wavenet
    this->suspendProcessing(true);
    
    if (amp_lead == 0) { // if lead on 
        WaveNetLoader loader2(BinaryData::bluej_fullD_p0153_json);
        float levelAdjust = loader2.levelAdjust;
        int numChannels2 = loader2.numChannels;
        int inputChannels2 = loader2.inputChannels;
        int outputChannels2 = loader2.outputChannels;
        int filterWidth2 = loader2.filterWidth;
        std::vector<int> dilations2 = loader2.dilations;
        std::string activation2 = loader2.activation;
        waveNet.setParams(inputChannels2, outputChannels2, numChannels2, filterWidth2, activation2,
            dilations2, levelAdjust);
        loader2.loadVariables(waveNet);
    } else { // else if clean on
        WaveNetLoader loader2(BinaryData::bluej_clean_p0088_json);
        float levelAdjust = loader2.levelAdjust;
        int numChannels2 = loader2.numChannels;
        int inputChannels2 = loader2.inputChannels;
        int outputChannels2 = loader2.outputChannels;
        int filterWidth2 = loader2.filterWidth;
        std::vector<int> dilations2 = loader2.dilations;
        std::string activation2 = loader2.activation;
        waveNet.setParams(inputChannels2, outputChannels2, numChannels2, filterWidth2, activation2,
            dilations2, levelAdjust);
        loader2.loadVariables(waveNet);
    }
    
    this->suspendProcessing(false);
}

void WaveNetVaAudioProcessor::loadConfig(File configFile)
{
    this->suspendProcessing(true);
    WaveNetLoader loader(dummyVar, configFile);
    float levelAdjust = loader.levelAdjust;
    int numChannels = loader.numChannels;
    int inputChannels = loader.inputChannels;
    int outputChannels = loader.outputChannels;
    int filterWidth = loader.filterWidth;
    std::vector<int> dilations = loader.dilations;
    std::string activation = loader.activation;
    waveNet.setParams(inputChannels, outputChannels, numChannels, filterWidth, activation,
        dilations, levelAdjust);
    loader.loadVariables(waveNet);
    this->suspendProcessing(false);
}

float WaveNetVaAudioProcessor::convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max)
{
    float b = log(y_max / y_min) / (x_max - x_min);
    float a = y_max / exp(b * x_max);
    float converted_value = a * exp(b * in_value);
    return converted_value;
}

void WaveNetVaAudioProcessor::set_ampCleanDrive(float db_ampCleanDrive)
{
    ampCleanDrive = decibelToLinear(db_ampCleanDrive);
    ampCleanGainKnobState = db_ampCleanDrive;
}

void WaveNetVaAudioProcessor::set_ampLeadDrive(float db_ampLeadDrive)
{
    ampLeadDrive = decibelToLinear(db_ampLeadDrive);
    ampLeadGainKnobState = db_ampLeadDrive;
}

void WaveNetVaAudioProcessor::set_ampMaster(float db_ampMaster)
{
    ampMaster = decibelToLinear(db_ampMaster);
    ampMasterKnobState = db_ampMaster;
}

void WaveNetVaAudioProcessor::set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider)
{
    eq4band.setParameters(bass_slider, mid_slider, treble_slider, presence_slider);

    ampPresenceKnobState = presence_slider;
}

float WaveNetVaAudioProcessor::decibelToLinear(float dbValue)
{
    return powf(10.0, dbValue/20.0);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveNetVaAudioProcessor();
}
