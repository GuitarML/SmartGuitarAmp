/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SmartAmpAudioProcessor::SmartAmpAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(CLEAN_GAIN_ID, CLEAN_GAIN_NAME, NormalisableRange<float>(-10.0f, 10.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(CLEAN_BASS_ID, CLEAN_BASS_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(CLEAN_MID_ID, CLEAN_MID_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(CLEAN_TREBLE_ID, CLEAN_TREBLE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(LEAD_GAIN_ID, LEAD_GAIN_NAME, NormalisableRange<float>(-10.0f, 10.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(LEAD_BASS_ID, LEAD_BASS_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(LEAD_MID_ID, LEAD_MID_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(LEAD_TREBLE_ID, LEAD_TREBLE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(PRESENCE_ID, PRESENCE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                    std::make_unique<AudioParameterFloat>(MASTER_ID, MASTER_NAME, NormalisableRange<float>(-36.0f, 0.0f, 0.01f), 0.0f) })

    
#endif
{
}

SmartAmpAudioProcessor::~SmartAmpAudioProcessor()
{
}

//==============================================================================
const String SmartAmpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SmartAmpAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SmartAmpAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SmartAmpAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SmartAmpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SmartAmpAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SmartAmpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SmartAmpAudioProcessor::setCurrentProgram (int index)
{
}

const String SmartAmpAudioProcessor::getProgramName (int index)
{
    return {};
}

void SmartAmpAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SmartAmpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    LSTM.reset();

    // prepare resampler for target sample rate: 44.1 kHz
    constexpr double targetSampleRate = 44100.0;
    resampler.prepareWithTargetSampleRate({ sampleRate, (uint32)samplesPerBlock, 1 }, targetSampleRate);

    if (amp_lead == 0 ) {
        MemoryInputStream jsonInputStream(BinaryData::BluesJr_json, BinaryData::BluesJr_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.load_json3(weights_json);
    } else {
        MemoryInputStream jsonInputStream(BinaryData::HT40_Overdrive_json, BinaryData::HT40_Overdrive_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.load_json3(weights_json);
    }

   

    // set up DC blocker
    dcBlocker.coefficients = dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 35.0f);
    dsp::ProcessSpec spec{ sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    dcBlocker.prepare(spec);

}

void SmartAmpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SmartAmpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SmartAmpAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    // Setup Audio Data
    const int numSamples = buffer.getNumSamples();
    const int numInputChannels = getTotalNumInputChannels();
    const int sampleRate = getSampleRate();


    // Amp =============================================================================
    if (amp_state == 1) {
        //    EQ (Presence, Bass, Mid, Treble)
        eq4band.process(buffer.getReadPointer(0), buffer.getWritePointer(0), midiMessages, numSamples, numInputChannels, sampleRate);


        // Apply ramped changes for gain smoothing
        if (ampDrive == previousAmpDrive)
        {
            buffer.applyGain(ampDrive);
        }
        else {
            buffer.applyGainRamp(0, numSamples, previousAmpDrive, ampDrive);
            previousAmpDrive = ampDrive;
        }

        // resample to target sample rate
        auto block = dsp::AudioBlock<float>(buffer.getArrayOfWritePointers(), 1, numSamples);
        auto block44k = resampler.processIn(block);

        // Apply LSTM model
        //LSTM.process(block44k.getChannelPointer(0), block44k.getChannelPointer(0), (int)block44k.getNumSamples());
        LSTM.process(block44k.getChannelPointer(0), gainValue, block44k.getChannelPointer(0), (int) block44k.getNumSamples());

        // resample back to original sample rate
        resampler.processOut(block44k, block);

        // Master Volume 
        // Apply ramped changes for gain smoothing
        if (ampMaster == previousAmpMaster)
        {
            buffer.applyGain(ampMaster);
        }
        else {
            buffer.applyGainRamp(0, (int)block44k.getNumSamples(), previousAmpMaster, ampMaster);
            previousAmpMaster = ampMaster;
        }

        // Custom Level for quieter models
        //if (current_model_index == 2) {
        //    buffer.applyGain(2.0);
        //}
    }

    // process DC blocker
    auto monoBlock = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    dcBlocker.process(dsp::ProcessContextReplacing<float>(monoBlock));

    // Handle stereo input by copying channel 1 to channel 2
    for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());
}

//==============================================================================
bool SmartAmpAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SmartAmpAudioProcessor::createEditor()
{
    return new SmartAmpAudioProcessorEditor (*this);
}

void SmartAmpAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    xml->setAttribute("amp_lead", amp_lead);
    xml->setAttribute("amp_state", amp_state);
    copyXmlToBinary(*xml, destData);
}

void SmartAmpAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(treeState.state.getType()))
        {
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
            amp_lead = xmlState->getIntAttribute("amp_lead");
            amp_state = xmlState->getIntAttribute("amp_state");
            
            /*
            switch (current_model_index)
            {
            case 0:
                //loadConfig(clean_tone);
                amp_lead = 0;
                break;
            case 1:
                //loadConfig(lead_tone);
                amp_lead = 1;
                break;
            }
            */
            //TODO fix
            //if (auto* editor = dynamic_cast<SmartAmpAudioProcessorEditor*> (getActiveEditor()))
            //    editor->resetImages();
        }
    }
}

/*
float SmartAmpAudioProcessor::convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max)
{
    float b = log(y_max / y_min) / (x_max - x_min);
    float a = y_max / exp(b * x_max);
    float converted_value = a * exp(b * in_value);
    return converted_value;
}
*/
void SmartAmpAudioProcessor::set_ampCleanDrive(float db_ampCleanDrive)
{
    //ampCleanDrive = decibelToLinear(db_ampCleanDrive);
    ampCleanGainKnobState = db_ampCleanDrive;
    gainValue = db_ampCleanDrive;
}

void SmartAmpAudioProcessor::set_ampLeadDrive(float db_ampLeadDrive)
{
    //ampLeadDrive = decibelToLinear(db_ampLeadDrive);
    ampLeadGainKnobState = db_ampLeadDrive;
    gainValue = db_ampLeadDrive;
}


void SmartAmpAudioProcessor::set_ampMaster(float db_ampMaster)
{
    //ampMaster = decibelToLinear(db_ampMaster);
    ampMasterKnobState = db_ampMaster;
}



void SmartAmpAudioProcessor::set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider)
{
    eq4band.setParameters(bass_slider, mid_slider, treble_slider, presence_slider);

    ampPresenceKnobState = presence_slider;
}

float SmartAmpAudioProcessor::decibelToLinear(float dbValue)
{
    return powf(10.0, dbValue/20.0);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SmartAmpAudioProcessor();
}
