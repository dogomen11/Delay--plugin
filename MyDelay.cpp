/*
  ==============================================================================

    MyDelay.cpp
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/

#include "MyDelay.h"


void MyDelay::fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length)
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


void MyDelay::getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
                                                  int m_delay_time, int m_sample_rate)
{
    const int read_position = static_cast<int> (delay_buffer_length + m_write_position - (m_sample_rate * m_delay_time / 1000)) % delay_buffer_length;
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

void  MyDelay::feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length)
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

