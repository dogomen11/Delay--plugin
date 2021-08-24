/*
  ==============================================================================

    MyDelay.h
    Created: 24 Aug 2021 12:47:21pm
    Author:  dogom

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

class MyDelay
{
    private:
        AudioBuffer<float> m_delay_buffer;
        int m_write_position;
        const float* buffer_data;
        const float* delay_buffer_data;
        float* dry_buffer;

        //const float* buffer_data = buffer.getReadPointer(channel);
        //const float* delay_buffer_data = m_delay_buffer.getReadPointer(channel);
        //float* dry_buffer = buffer.getWritePointer(channel);
  
    public:
        MyDelay (AudioBuffer<float> m_delay_buffer, int m_write_position, const float* buffer_read, const float* delay_buffer_read, float* buffer_write)
                               : m_delay_buffer(m_delay_buffer), m_write_position(m_write_position),
                                 buffer_data(buffer_read), delay_buffer_data(delay_buffer_read), dry_buffer(buffer_write) {}
        ~MyDelay() = default;

        void fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length, float m_delay_mix);

        void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
                                int m_delay_time, int m_sample_rate);

        void feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length, float m_delay_mix);
};
