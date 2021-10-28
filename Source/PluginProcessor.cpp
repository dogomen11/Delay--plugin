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
#endif
{

    NormalisableRange<float> m_input_gain_range(-60.0f, 12.0f, 0.01f);
    parameters.createAndAddParameter(INPUT_GAIN_ID, INPUT_GAIN_NAME, INPUT_GAIN_NAME, m_input_gain_range, 0.0f, nullptr, nullptr);
    NormalisableRange<float> m_output_gain_range(-60.0f, 12.0f, 0.01f);
    parameters.createAndAddParameter(OUTPUT_GAIN_ID, OUTPUT_GAIN_NAME, OUTPUT_GAIN_NAME, m_output_gain_range, 0.0f, nullptr, nullptr);
    NormalisableRange<float> m_delay_mix__range(0.0f, 1.0f, 0.01f);
    parameters.createAndAddParameter(DELAY_MIX_ID, DELAY_MIX_NAME, DELAY_MIX_NAME, m_delay_mix__range, 0.5f, nullptr, nullptr);

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

void NewProjectAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName(int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//================================================================================================================================
void NewProjectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    const int num_input_channels = getNumInputChannels();
    const int delay_buffer_size = 2 * (sampleRate + samplesPerBlock);
    m_delay_buffer.setSize(num_input_channels, delay_buffer_size);

    last_sample_rate = sampleRate;
    m_sample_rate = sampleRate;

    previous_gain = pow(10, *parameters.getRawParameterValue(INPUT_GAIN_ID) / 20);
    m_output_gain = pow(10, *parameters.getRawParameterValue(OUTPUT_GAIN_ID) / 20);
    m_delay_mix = *parameters.getRawParameterValue(DELAY_MIX_ID);

    dsp::ProcessSpec spec;
    spec.sampleRate = last_sample_rate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    for (int i = 0; i < NUM_OF_INSTENCES; i++)
    {
        m_delay_panner[i].setRule(PannerRule::balanced);
        m_delay_panner[i].prepare(spec);
        m_delay_panner[i].setPan(m_pan_dials[i]);
    }

    updateParameters();
    m_visualiser.clear();

}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
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

}

void NewProjectAudioProcessor::process(dsp::ProcessContextReplacing<float> context)
{

}

//================================================================================================================================
void NewProjectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    float current_gain = pow(10, *parameters.getRawParameterValue(INPUT_GAIN_ID) / 20);
    if (current_gain == previous_gain)
    {
        buffer.applyGain(current_gain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previous_gain, current_gain);
    }


    current_delay.updateArgs(m_write_position, m_on_off_button_array, m_delay_mix, m_delay_time);
    const int buffer_length = buffer.getNumSamples();
    const int delay_buffer_length = m_delay_buffer.getNumSamples();


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // input gain change         
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(m_input_gain);

        }
        const float* buffer_data = buffer.getReadPointer(channel);
        const float* delay_buffer_data = m_delay_buffer.getReadPointer(channel);
        //const float* my_delay_buffer_data = current_delay.getReadPointer(channel);
        float* dry_buffer = buffer.getWritePointer(channel);
        marked = current_delay.isMarked();
        m_reverb.setInputBuffer(buffer);
        m_reverb.setupMyReverb();

        if (marked == 0)
        {
            fillDelayBuffer(channel, buffer_length, delay_buffer_length, buffer_data, delay_buffer_data, m_delay_mix);
            getFromDelayBuffer(buffer, channel, buffer_length, delay_buffer_length, buffer_data, delay_buffer_data, m_delay_time);
            feedbackDelay(channel, buffer_length, delay_buffer_length, dry_buffer, m_delay_mix);
        }
        else
        {
            buffer.makeCopyOf(m_reverb.addReverb());
        }
    }

    m_visualiser.pushBuffer(buffer);

    // output gain change
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);               
        channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = buffer.getSample(channel, sample) * juce::Decibels::decibelsToGain(m_output_gain);
        }
    }


    /*
    dsp::AudioBlock<float> block(buffer);
    for (int i = 0; i < 16; i++)
    {
        m_delay_panner[i].process(ProcessContextReplacing<float>(block));
        //m_reverb.setEnabled(true);
        //m_reverb.process(ProcessContextReplacing<float>(block));
    }
    */
    m_write_position += buffer_length;
    m_write_position %= delay_buffer_length;

}


//copy from buffer to delay_buffer
void NewProjectAudioProcessor::fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length,
                                                const float* buffer_data, const float* delay_buffer_data, float m_delay_mix)
{
    if (delay_buffer_length > buffer_length + m_write_position)
    {
        m_delay_buffer.copyFromWithRamp(channel, m_write_position, buffer_data, buffer_length, m_delay_mix, m_delay_mix);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - m_write_position;
        m_delay_buffer.copyFromWithRamp(channel, m_write_position, buffer_data, buffer_remaining, m_delay_mix, m_delay_mix);
        m_delay_buffer.copyFromWithRamp(channel, 0, buffer_data, (buffer_length - buffer_remaining), m_delay_mix, m_delay_mix);
    }
}







void NewProjectAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
    const float* buffer_data, const float* delay_buffer_data, int m_delay_time)
{
    const int read_position = static_cast<int> (delay_buffer_length + m_write_position - (m_sample_rate * m_delay_time / 1000)) % delay_buffer_length;
    /*//TODO: try to use instence position everytime ime filling the buffer again, and do an effect before the buffer fill
    //
    //
    //
    if (m_on_off_button_array[instence_position] == true)
    {
        dsp::AudioBlock<float> temp_delay_block(buffer);
        //
    }
    //
    /*/
    if (delay_buffer_length > buffer_length + read_position)
    {
        buffer.addFrom(channel, 0, delay_buffer_data + read_position, buffer_length);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - read_position;
        buffer.copyFrom(channel, 0, delay_buffer_data + read_position, buffer_remaining);
        buffer.copyFrom(channel, buffer_remaining, delay_buffer_data, buffer_length - buffer_remaining);
    }
    instence_position = (instence_position + 1) % 16;
}

void NewProjectAudioProcessor::feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length,
    float* dry_buffer, float m_delay_mix)
{
    if (delay_buffer_length > buffer_length + m_write_position)
    {
        m_delay_buffer.addFromWithRamp(channel, m_write_position, dry_buffer, buffer_length, m_delay_mix, m_delay_mix);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - m_write_position;
        m_delay_buffer.addFromWithRamp(channel, buffer_remaining, dry_buffer, buffer_remaining, m_delay_mix, m_delay_mix);
        m_delay_buffer.addFromWithRamp(channel, 0, dry_buffer, buffer_length - buffer_remaining, m_delay_mix, m_delay_mix);
    }
}


//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor(*this);
}


void NewProjectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    std::unique_ptr<XmlElement> xml_save(parameters.state.createXml());
    copyXmlToBinary(*xml_save, destData);

}

void NewProjectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<XmlElement> the_parameters(getXmlFromBinary(data, sizeInBytes));
    if (the_parameters != nullptr)
    {
        if (the_parameters->hasTagName(parameters.state.getType()))
        {
            parameters.state = ValueTree::fromXml(*the_parameters);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}