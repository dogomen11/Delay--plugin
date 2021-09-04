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

#define NUM_OF_INSTENCES 16

class MyDelay
{
    private:
        AudioBuffer<float> m_delay_buffer;
        int m_write_position;
        const float* buffer_data;
        const float* delay_buffer_data;
        float* dry_buffer;
        int instences[NUM_OF_INSTENCES] = {};
        int marked_instences = 0;
        float m_delay_mix;
        float m_output_gain;

        //const float* buffer_data = buffer.getReadPointer(channel);
        //const float* delay_buffer_data = m_delay_buffer.getReadPointer(channel);
        //float* dry_buffer = buffer.getWritePointer(channel);
  
    public:
        MyDelay (AudioBuffer<float> m_delay_buffer, int m_write_position, const float* buffer_read, const float* delay_buffer_read, float* buffer_write, bool m_on_off_button_array[], float m_delay_mix = 0.5f, float m_output_gain = 0.0f) :
                                 m_delay_buffer(m_delay_buffer), m_write_position(m_write_position),
                                 buffer_data(buffer_read), delay_buffer_data(delay_buffer_read), dry_buffer(buffer_write),
                                 m_delay_mix(m_delay_mix) ,m_output_gain(m_output_gain) 
        {
            for (int i = 0; i < 16; i++)
            {
                instences[i] = m_on_off_button_array[i];
                if (m_on_off_button_array[i] == true)
                {
                    marked_instences++;
                }
            }
        }
        ~MyDelay() = default;

        void fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length);

        void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
                                 int m_delay_time, int m_sample_rate);

        void feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length);

        void addInstence(int instance_num);
        void decreseInstence(int instance_num);

        int isMarked()                     { return marked_instences; }
};
