/*
  ==============================================================================

    Filter.h
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/

#ifndef FILTER
#define FILTER
#include <math.h>
#include <stdio.h>
#include <JuceHeader.h>

using namespace juce;

enum filter_logic
{
    TWO_TERM_DIFF_FILTER,
    TWO_TERM_AVG_FILTER,
    THREE_TERM_AVG_FILTER,
    CENTRAL_DIFF_FILTER,
    RECURSIVR_FILTER
};

enum filter_type
{
    LOW_PASS,
    HIGH_PASS,
    BAND_PASS
};


class Filter
{
    protected:
        float f_buffer_size;
        AudioBuffer<float>& in_buffer;
        AudioBuffer<float>& out_buffer;
        float coefficients[5];
        void setLowPass();
        void setHighPass();
        void setBandPass();
    public:
        Filter(AudioBuffer<float>& buffer, filter_logic new_logic);
        ~Filter() = default;
        void setFilterType(filter_type new_type);
        virtual AudioBuffer<float> applyFilter(int channel) = 0;
};



class TwoTermDifferenceFilter : public Filter
{
    public:
        TwoTermDifferenceFilter(AudioBuffer<float>& buffer) : Filter(buffer) {}
        virtual ~TwoTermDifferenceFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

class TwoTermAverageFilter : public Filter
{
    public:
        TwoTermAverageFilter(AudioBuffer<float>& buffer) : Filter(buffer) {}
        virtual ~TwoTermAverageFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

class ThreeTermAverageFilter : public Filter
{
    public:
        ThreeTermAverageFilter(AudioBuffer<float>& buffer) : Filter(buffer) {}
        virtual ~ThreeTermAverageFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

class CenteralDifferenceFilter : public Filter
{
    public:
        CenteralDifferenceFilter(AudioBuffer<float>& buffer) : Filter(buffer) {}
        virtual ~CenteralDifferenceFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};


class RecursiveFilter : public Filter
{
    public:
        RecursiveFilter(AudioBuffer<float>& buffer) : Filter(buffer) {}
        virtual ~RecursiveFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

#endif