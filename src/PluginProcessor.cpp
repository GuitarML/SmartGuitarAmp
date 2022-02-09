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

    // prepare resampler for target sample rate: 48 kHz
    constexpr double targetSampleRate = 48000.0;
    resampler.prepareWithTargetSampleRate ({ sampleRate, (uint32) samplesPerBlock, 1 }, targetSampleRate);

    // load 48 kHz sample rate model
    MemoryInputStream jsonInputStream(BinaryData::model_ts9_48k_cond2_json, BinaryData::model_ts9_48k_cond2_jsonSize, false);
    nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());

    LSTM.load_json3(weights_json);

    // set up DC blocker
    //dcBlocker.coefficients = dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 35.0f);
    //dsp::ProcessSpec spec{ sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    //dcBlocker.prepare(spec);

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


        //    Master Volume 
        buffer.applyGain(ampMaster);


    }
    
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

//==============================================================================
void SmartAmpAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SmartAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}



float SmartAmpAudioProcessor::convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max)
{
    float b = log(y_max / y_min) / (x_max - x_min);
    float a = y_max / exp(b * x_max);
    float converted_value = a * exp(b * in_value);
    return converted_value;
}

void SmartAmpAudioProcessor::set_ampCleanDrive(float db_ampCleanDrive)
{
    ampCleanDrive = decibelToLinear(db_ampCleanDrive);
    ampCleanGainKnobState = db_ampCleanDrive;
}

void SmartAmpAudioProcessor::set_ampLeadDrive(float db_ampLeadDrive)
{
    ampLeadDrive = decibelToLinear(db_ampLeadDrive);
    ampLeadGainKnobState = db_ampLeadDrive;
}

void SmartAmpAudioProcessor::set_ampMaster(float db_ampMaster)
{
    ampMaster = decibelToLinear(db_ampMaster);
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
