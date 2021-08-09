/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;
using namespace juce::dsp;

//==============================================================================

//contructor
NewProjectAudioProcessor::NewProjectAudioProcessor()
        #ifndef JucePlugin_PreferredChannelConfigurations
            : AudioProcessor(BusesProperties()
        #if ! JucePlugin_IsMidiEffect
        #if ! JucePlugin_IsSynth
                .withInput("Input", juce::AudioChannelSet::stereo(), true)
        #endif
                .withOutput("Output", juce::AudioChannelSet::stereo(), true)
        #endif
            ),
            parameters(*this, nullptr)
            //low_pass_filter(dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1f) )
        #endif
{
    NormalisableRange<float> gain_range(-12.0f, 12.0f, 0.01f);
    NormalisableRange<float> cutoffRange(20.0f, 20000.0f, 0.01f);
    NormalisableRange<float> resRange(1.0f, 5.0f, 0.001f);

    parameters.createAndAddParameter(GAIN_ID, GAIN_NAME, GAIN_NAME,  gain_range, 0.5f, nullptr, nullptr);
    parameters.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0f, nullptr, nullptr);
    parameters.createAndAddParameter("resonance", "Resonance", "resonance", resRange, 1.0f, nullptr, nullptr);

    parameters.state = juce::ValueTree("saved_parameters");
}

//destructor
NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//================================================================================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//================================================================================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    last_sample_rate = sampleRate;

    previous_gain = pow(10, *parameters.getRawParameterValue(GAIN_ID) / 20);

    dsp::ProcessSpec spec;
    spec.sampleRate = last_sample_rate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    //low_pass_filter.prepare(spec);
    //low_pass_filter.reset();
    state_variable_filter.reset();
    updateParameters();
    state_variable_filter.prepare(spec);
    
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

//================================================================================================================================
void NewProjectAudioProcessor::updateParameters()
{
    float low_pass_f_frequency = *parameters.getRawParameterValue("cutoff");
    float low_pass_f_resonance = *parameters.getRawParameterValue("resonance");

    //*low_pass_filter.state = *dsp::IIR::Coefficients<float>::makeLowPass(last_sample_rate, low_pass_f_frequency, low_pass_f_resonance);
    state_variable_filter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
    state_variable_filter.state->setCutOffFrequency(last_sample_rate, low_pass_f_frequency, low_pass_f_resonance);
}

void NewProjectAudioProcessor::process(dsp::ProcessContextReplacing<float> context)
{

}

//================================================================================================================================
void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float current_gain = pow(10, *parameters.getRawParameterValue(GAIN_ID) / 20);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (current_gain == previous_gain)
    {
        buffer.applyGain(current_gain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previous_gain, current_gain);
    }
    
    dsp::AudioBlock<float> block(buffer);

    updateParameters();
    state_variable_filter.process(dsp::ProcessContextReplacing<float>(block) );

    //process(dsp::ProcessContextReplacing<float>(block));


    //low_pass_filter.process(dsp::ProcessContextReplacing<float>(block));

    
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(mGain);

        }
    }
    */
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    std::unique_ptr<XmlElement> xml_save(parameters.state.createXml());
    copyXmlToBinary(*xml_save, destData);

}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<XmlElement> the_parameters( getXmlFromBinary(data, sizeInBytes) );
    if (the_parameters != nullptr)
    {
        if (the_parameters->hasTagName(parameters.state.getType() ) )
        {
            parameters.state = ValueTree::fromXml(*the_parameters);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
