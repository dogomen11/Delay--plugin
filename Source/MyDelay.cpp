/*
  ==============================================================================

    MyDelay.cpp
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/

#include "MyDelay.h"

MyDelay::MyDelay() : write_position(0.0),
                    marked_instences(0),
                    delay_mix(0.0),
                    delay_time(0.3),
                    input_gain(1.0),
                    output_gain(1.0) 
{
    delay_buffer.clear();
    delay_panner.reset();
}

MyDelay::~MyDelay() {}

void MyDelay::setDelayMix(float delay_mix_chosen)
{
    delay_mix = delay_mix_chosen;
}

void MyDelay::setDelayTime(int delay_time_chosen)
{
    delay_time = delay_time_chosen;
}

void MyDelay::setInputGain(float in_gain_chosen)
{
    input_gain = in_gain_chosen;
}

void MyDelay::setOutputGain(float out_gain_chosen)
{
    input_gain = out_gain_chosen;
}

void MyDelay::setInputBuffer(AudioBuffer<float>& new_buffer)
{
    setSize(new_buffer.getNumChannels(), new_buffer.getNumSamples());
}

void MyDelay::setSize(int new_num_channels, int new_num_samples)
{
    delay_buffer.setSize(new_num_channels, new_num_samples);
    delay_buffer_length = delay_buffer.getNumSamples();
}

void MyDelay::updateArgs(int m_write_position, int m_sample_rate, bool m_on_off_button_array[], float m_delay_mix, int m_delay_time)
{
    write_position = m_write_position;
    sample_rate = m_sample_rate;
    delay_mix = m_delay_mix;
    delay_time = m_delay_time;
    marked_instences = 0;
    for (int i = 0; i < NUM_OF_INSTENCES; i++)
    {
        instences[i] = m_on_off_button_array[i];
        if (m_on_off_button_array[i] == true)
        {
            marked_instences++;
        }
    }
}

int MyDelay::getNumSamples()
{
    return delay_buffer.getNumSamples();
}

const float* MyDelay::getReadPointer(int channelNumber)
{
    return delay_buffer.getReadPointer(channelNumber);
}

void MyDelay::setPannerSpec(dsp::ProcessSpec new_spec)
{
    delay_panner.prepare(new_spec);
    delay_panner.setRule(juce::dsp::PannerRule::balanced);
}

void MyDelay::setPannerValue(float current_pan)
{
    delay_panner.setPan(current_pan);
}

void MyDelay::processPan(AudioBuffer<float>& buffer_to_pan)
{
    juce::dsp::AudioBlock<float> block(buffer_to_pan);
    juce::dsp::ProcessContextReplacing content(block);
    delay_panner.process(content);
}


void MyDelay::fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data)
{
    /*
    for (int i = 0; i < NUM_OF_INSTENCES; i++)
    {
        delay_buffer.copyFromWithRamp(channel, (i * buffer_length), buffer_data, buffer_length, input_gain, output_gain);
        auto* channelData = delay_buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer_length; ++sample)
        {
            channelData[sample] = delay_buffer.getSample(channel, sample) * juce::Decibels::decibelsToGain(instences_volume[i]);
        }
    }*/
    if (delay_buffer_length > buffer_length + write_position)
    {
        delay_buffer.copyFromWithRamp(channel, write_position, buffer_data, buffer_length, delay_mix, delay_mix);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - write_position;
        delay_buffer.copyFromWithRamp(channel, write_position, buffer_data, buffer_remaining, delay_mix, delay_mix);
        delay_buffer.copyFromWithRamp(channel, 0, buffer_data, (buffer_length - buffer_remaining), delay_mix, delay_mix);
    }
}


void MyDelay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, float* dry_buffer, float vol_dials[], float m_pan_dials[])
{
    /*
    const int read_position = static_cast<int> (delay_buffer_length + write_position - (sample_rate * delay_time / 1000)) % delay_buffer_length;
    const float* delay_buffer_data = delay_buffer.getReadPointer(channel);
    const float* instence_to_copy = delay_buffer_data + (outputing_stage * buffer_length);
    jassert(outputing_stage <= 15 && buffer_length <= delay_buffer_length);


    buffer.addFrom(channel, 0, instence_to_copy, buffer_length);
    */
    const int read_position = static_cast<int> (delay_buffer_length + write_position - (sample_rate * delay_time / 1000)) % delay_buffer_length;
    AudioBuffer temp(delay_buffer);
    auto* channelData = temp.getWritePointer(channel);
    channelData = temp.getWritePointer(channel);
    for (int sample = 0; sample < temp.getNumSamples(); ++sample)
    {
        if (instences[outputing_stage] == 1)
        {
            channelData[sample] = temp.getSample(channel, sample) * vol_dials[outputing_stage];
        }
        else
        {
            //temp.clear();
            int debug = instences[outputing_stage];
            channelData[sample] = temp.getSample(channel, sample) * 0;
        }
    }
    setPannerValue(m_pan_dials[outputing_stage]);
    //processPan(temp);
    if (delay_buffer_length > buffer_length + read_position)
    {
        buffer.addFrom(channel, 0, temp.getReadPointer(channel) + read_position, buffer_length);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - read_position;
        const float* delay_buffer_data = temp.getReadPointer(channel);
        buffer.copyFrom(channel, 0, delay_buffer_data + read_position, buffer_remaining);
        buffer.copyFrom(channel, buffer_remaining, delay_buffer_data, buffer_length - buffer_remaining);
    }
    feedbackDelay(channel, buffer_length, dry_buffer);
    time_strecher++;
    float debug_2 = sample_rate/512;     //TODO change time strech formula
    if (channel == 1  &&  time_strecher >= debug_2 )
    {
        outputing_stage = (outputing_stage + 1) % 16;
        time_strecher = 0;
    }
}

void MyDelay::feedbackDelay(int channel, const int buffer_length, float* dry_buffer)
{
    if (delay_buffer_length > buffer_length + write_position)
    {
        delay_buffer.addFromWithRamp(channel, write_position, dry_buffer, buffer_length, delay_mix, delay_mix);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - write_position;
        delay_buffer.addFromWithRamp(channel, buffer_remaining, dry_buffer, buffer_remaining, delay_mix, delay_mix);
        delay_buffer.addFromWithRamp(channel, 0, dry_buffer, buffer_length - buffer_remaining, delay_mix, delay_mix);
    }
}


void MyDelay::addInstence(int instance_num)
{
    marked_instences++;
    instences[instance_num] = 1;
    //mark instens and make him play delayed sample
}

void MyDelay::decreseInstence(int instance_num)
{
    marked_instences--;
    instences[instance_num] = 0;
    //unmark instens and make him NOT play delayed sample 
}

int MyDelay::isMarked()
{
    return marked_instences;
}