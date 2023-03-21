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
    waveNet(1, 1, 1, 1, "linear", { 1 }),
    treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(CLEAN_GAIN_ID, CLEAN_GAIN_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f),
                        std::make_unique<AudioParameterFloat>(CLEAN_BASS_ID, CLEAN_BASS_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(CLEAN_MID_ID, CLEAN_MID_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(CLEAN_TREBLE_ID, CLEAN_TREBLE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        
                        std::make_unique<AudioParameterFloat>(LEAD_GAIN_ID, LEAD_GAIN_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f),
                        std::make_unique<AudioParameterFloat>(LEAD_BASS_ID, LEAD_BASS_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(LEAD_MID_ID, LEAD_MID_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(LEAD_TREBLE_ID, LEAD_TREBLE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
   
                        std::make_unique<AudioParameterFloat>(PRESENCE_ID, PRESENCE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(MASTER_ID, MASTER_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f) })

#endif
{
    loadConfigAmp();

    cleanBassParam = treeState.getRawParameterValue (CLEAN_BASS_ID);
    cleanMidParam = treeState.getRawParameterValue (CLEAN_MID_ID);
    cleanTrebleParam = treeState.getRawParameterValue (CLEAN_TREBLE_ID);
    cleanGainParam = treeState.getRawParameterValue (CLEAN_GAIN_ID);
    leadGainParam = treeState.getRawParameterValue (LEAD_GAIN_ID);
    leadBassParam = treeState.getRawParameterValue (LEAD_BASS_ID);
    leadMidParam = treeState.getRawParameterValue (LEAD_MID_ID);
    leadTrebleParam = treeState.getRawParameterValue (LEAD_TREBLE_ID);
    presenceParam = treeState.getRawParameterValue (PRESENCE_ID);
    masterParam = treeState.getRawParameterValue (MASTER_ID);	


    auto cleanBassValue = static_cast<float> (cleanBassParam->load());
    auto cleanMidValue = static_cast<float> (cleanMidParam->load());
    auto cleanTrebleValue = static_cast<float> (cleanTrebleParam->load());

    auto leadBassValue = static_cast<float> (leadBassParam->load());
    auto leadMidValue = static_cast<float> (leadMidParam->load());
    auto leadTrebleValue = static_cast<float> (leadTrebleParam->load());

    auto presenceValue = static_cast<float> (presenceParam->load());

    if (amp_lead == 0) {
        eq4band.setParameters(cleanBassValue, cleanMidValue, cleanTrebleValue, presenceValue);
    } else {
        eq4band.setParameters(leadBassValue, leadMidValue, leadTrebleValue, presenceValue);
    }
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
    const int sampleRate = getSampleRate();

    auto cleanGainValue = static_cast<float> (cleanGainParam->load());
    auto leadGainValue = static_cast<float> (leadGainParam->load());

    auto presenceValue = static_cast<float> (presenceParam->load());
    auto masterValue = static_cast<float> (masterParam->load());

    // Amp =============================================================================
    if (amp_state == 1) {

        if (amp_lead == 0) {// if clean channel eq/gain
            buffer.applyGain(cleanGainValue * 8.0);
        }
        else {// else lead channel eq/gain
            buffer.applyGain(leadGainValue * 8.0);
        }

        //    Wavenet, load json for waveNet2 based on lead/clean switch
        waveNet.process(buffer.getArrayOfReadPointers(), buffer.getArrayOfWritePointers(), buffer.getNumSamples());

        eq4band.process(buffer.getReadPointer(0), buffer.getWritePointer(0), midiMessages, numSamples, numInputChannels, sampleRate);

        //    Master Volume 
        buffer.applyGain(masterValue);

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
    return new WrappedWaveNetVaAudioProcessorEditor (*this);
}

//==============================================================================
void WaveNetVaAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    xml->setAttribute ("amp_state", amp_state);
    xml->setAttribute ("amp_lead", amp_lead);
    xml->setAttribute ("gui_scale_factor", gui_scale_factor);
    copyXmlToBinary (*xml, destData);
}

void WaveNetVaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (treeState.state.getType()))
        {
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
            amp_state = xmlState->getBoolAttribute ("amp_state");
            amp_lead = xmlState->getBoolAttribute ("amp_lead");
            gui_scale_factor = xmlState->getDoubleAttribute ("gui_scale_factor", 1.0);
            if (auto* editor = dynamic_cast<WrappedWaveNetVaAudioProcessorEditor*> (getActiveEditor()))
                editor->resetImages();
        }
    }
}


void WaveNetVaAudioProcessor::loadConfigAmp() 
{
    // Load Second Wavenet
    this->suspendProcessing(true);
    
    if (amp_lead == 1) { // if lead on 
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


void WaveNetVaAudioProcessor::set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider)
{
    eq4band.setParameters(bass_slider, mid_slider, treble_slider, presence_slider);
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveNetVaAudioProcessor();
}
