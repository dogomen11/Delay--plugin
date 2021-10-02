/*
  ==============================================================================

    MyDelay.h
    Created: 24 Aug 2021 12:47:21pm
    Author:  dogom

  ==============================================================================
*/
#ifndef MYDELAY
#define MYDELAY
#pragma once
#include <JuceHeader.h>
using namespace juce;

#define NUM_OF_INSTENCES 16

class MyDelay
{
    private:
        AudioBuffer<float> delay_buffer;
        int write_position = 0.0f;
        int instences[NUM_OF_INSTENCES] = {};
        int marked_instences = 0;
        float delay_mix = 0.0f;
        int delay_time =  0.0f;

    public:
        MyDelay() : delay_buffer() {}

        ~MyDelay() = default;

        void updateArgs(int m_write_position, bool m_on_off_button_array[], float m_delay_mix, int m_delay_time);

        void setSize(int new_num_channels, int new_num_samples);

        int getNumSamples();

        const float* getReadPointer(int channelNumber);

        void fillDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length,
            const float* buffer_data, const float* delay_buffer_data);

        void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, const int delay_buffer_length,
            const float* buffer_data, const float* delay_buffer_data, int m_sample_rate);

        void feedbackDelay(int channel, const int buffer_length, const int delay_buffer_length,
            float* dry_buffer);

        void fillFirstDelayBuffer(int channel, const int buffer_length, const int delay_buffer_length,
            const float* buffer_data, const float* delay_buffer_data, float instences_volume[]);

        void addInstence(int instance_num);
        void decreseInstence(int instance_num);

        int isMarked()                     { return marked_instences; }
};

#endif     // MYDELAY