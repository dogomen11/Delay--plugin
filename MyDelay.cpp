/*
  ==============================================================================

    MyDelay.cpp
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/

#include "MyDelay.h"

void MyDelay::updateArgs(int m_write_position, bool m_on_off_button_array[], float m_delay_mix, int m_delay_time)
{
    write_position = m_write_position;
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

void MyDelay::setSize(int new_num_channels, int new_num_samples)
{
    int new_delay_size = new_num_samples * NUM_OF_INSTENCES;
    delay_buffer.setSize(new_num_channels, new_delay_size);
}

int MyDelay::getNumSamples()
{
    return delay_buffer.getNumSamples();
}

const float* MyDelay::getReadPointer(int channelNumber)
{
    return delay_buffer.getReadPointer(channelNumber);
}


/*
void MyDelay::fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length, const float* buffer_data, const float* delay_buffer_data)
{
    if (delay_buffer_length > buffer_length + write_position)
    {
        delay_buffer[0].copyFromWithRamp(channel, write_position, buffer_data, buffer_length, delay_mix, delay_mix);
    }
    else
    {
        const int buffer_remaining = delay_buffer_length - write_position;
        delay_buffer[0].copyFromWithRamp(channel, write_position, buffer_data, buffer_remaining, delay_mix, delay_mix);
        delay_buffer[0].copyFromWithRamp(channel, 0, buffer_data, (buffer_length - buffer_remaining), delay_mix, delay_mix);
    }
}


void MyDelay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
                                  const float* buffer_data, const float* delay_buffer_data, int m_sample_rate)
{
    const int read_position = static_cast<int> (delay_buffer_length + write_position - (m_sample_rate * delay_time / 1000)) % delay_buffer_length;
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
}

void  MyDelay::feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length, float* dry_buffer)
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

*/

void MyDelay::fillFirstDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length,
                                    const float* buffer_data, const float* delay_buffer_data, float instences_volume[])
{
    for (int i = 0; i < NUM_OF_INSTENCES; i++)
    {
        delay_buffer.copyFromWithRamp(channel, write_position + (i * buffer_length), delay_buffer.getReadPointer(channel), buffer_length, 1, 1);
        auto* channelData = delay_buffer.getWritePointer(channel);
        for (int sample = 0; sample < delay_buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = delay_buffer.getSample(channel, sample) * juce::Decibels::decibelsToGain(instences_volume[i]);
        }
    }
}

void MyDelay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
                                    const float* buffer_data, const float* delay_buffer_data, int m_sample_rate)
{
    const int read_position = static_cast<int> (delay_buffer_length + write_position - (m_sample_rate * delay_time / 1000)) % delay_buffer_length;
    if (delay_buffer_length > buffer_length + read_position)
    {
        buffer.addFrom(channel, 0, delay_buffer_data + read_position, buffer_length);
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

