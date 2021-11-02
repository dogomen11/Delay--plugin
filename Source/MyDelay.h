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

//==============================================================================

class MyDelay
{
private:

    // Delay initial parameters

    int write_position;
    int sample_rate;
    int instences[NUM_OF_INSTENCES] = {};
    int marked_instences;
    float delay_mix;
    int delay_time;
    float input_gain;
    float output_gain;
    AudioBuffer<float> delay_buffer;
    int delay_buffer_length = 0;
    int outputing_stage = 0;
    int time_strecher = 0;
    juce::dsp::Panner<float> delay_panner;

public:

    MyDelay();
    ~MyDelay();

    void setDelayMix(float delay_mix_chosen);
    void setDelayTime(int delay_time_chosen);
    void setInputGain(float in_gain_chosen);
    void setOutputGain(float out_gain_chosen);
    void setPannerSpec(dsp::ProcessSpec new_spec);
    void setPannerValue(float current_pan);
    void processPan(AudioBuffer<float>& buffer_to_pan);

    void setInputBuffer(AudioBuffer <float>& new_buffer);

    void updateArgs(int m_write_position, int m_sample_rate, bool m_on_off_button_array[], float m_delay_mix, int m_delay_time);
    void setSize(int new_num_channels, int new_num_samples);
    int getNumSamples();
    const float* getReadPointer(int channelNumber);

    void fillDelayBuffer(int channel, const int buffer_length, const float* buffer_data);

    void getFromDelayBuffer(AudioBuffer<float>& buffer, int channel, const int buffer_length, float vol_dials[], float m_pan_dials[]);

    void feedbackDelay(int channel, const int buffer_length, float* dry_buffer);

    

    void addInstence(int instance_num);
    void decreseInstence(int instance_num);
    int isMarked();
};

#endif     // MYDELAY