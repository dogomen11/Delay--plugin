/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"

using namespace juce;
using namespace juce::dsp;


class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;


    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    float m_gaim = 0.0;
    float previous_gain;

    void process(dsp::ProcessContextReplacing<float> context);
    void updateParameters();
    juce::AudioProcessorValueTreeState parameters;

    void fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length, const float* buffer_data, const float* delay_buffer_data);

    void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
        const float* buffer_data, const float* delay_buffer_data);

    void feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length,
                       float* dry_buffer);

private:

    float last_sample_rate;

    AudioBuffer<float> m_delay_buffer;
    int m_write_position{ 0 };
    int m_sample_rate{ 44100 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
