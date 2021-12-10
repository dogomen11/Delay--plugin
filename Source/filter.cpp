/*
  ==============================================================================

    Filter.cpp
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/
#include "filter.h"


Filter::Filter(AudioBuffer<float>& buffer, filter_logic new_logic) : in_buffer(buffer), out_buffer(buffer)
{
    f_buffer_size = buffer.getNumSamples();
    out_buffer.setSize(buffer.getNumChannels(), buffer.getNumSamples());
}

void Filter::setFilterType(filter_type new_type)
{
    switch (new_type)
    {
        case (LOW_PASS):
            setLowPass();
            break;
        case (HIGH_PASS):
            setHighPass();
            break;
        case: (BAND_PASS) :
            setBandPass();
            break;
    }
}

void Filter::setLowPass()
{

}

void Filter::setHighPass()
{

}

void Filter::setBandPass()
{

}

AudioBuffer<float> TwoTermDifferenceFilter::applyFilter(int channel)
{
    float current_difference, first, second;
    for (int i = 1; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 1);
        second = in_buffer.getSample(channel, i);
        current_difference = second - first;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}

AudioBuffer<float> TwoTermAverageFilter::applyFilter(int channel)
{
    float current_difference, first, second;
    for (int i = 1; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 1);
        second = in_buffer.getSample(channel, i);
        current_difference = (second + first) / 2;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}


AudioBuffer<float> ThreeTermAverageFilter::applyFilter(int channel)
{
    float current_difference, first, second, third;
    for (int i = 2; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 2);
        second = in_buffer.getSample(channel, i - 1);
        third = in_buffer.getSample(channel, i);
        current_difference = (third + second + first) / 3;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}

AudioBuffer<float> CenteralDifferenceFilter::applyFilter(int channel)
{
    float current_difference, first, second;
    for (int i = 2; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 2);
        second = in_buffer.getSample(channel, i);
        current_difference = (second - first) / 2;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}

AudioBuffer<float> RecursiveFilter::applyFilter(int channel)
{
    float current_difference, first, second, third;
    float rec_first, rec_second;
    for (int i = 2; i < f_buffer_size; i++)
    {
        first = in_buffer.getSample(channel, i - 2);
        second = in_buffer.getSample(channel, i - 1);
        first = in_buffer.getSample(channel, i);
        rec_first = out_buffer.getSample(channel, i - 1);
        rec_second = out_buffer.getSample(channel, i - 2);
        current_difference = (third + second + first - rec_first - rec_second) / 3;
        out_buffer.setSample(channel, i, current_difference);
    }
    return out_buffer;
}


