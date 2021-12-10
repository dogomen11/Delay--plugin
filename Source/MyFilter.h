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
#define M_PI 3.14159265358979323846264338327950288

enum filter_logic
{
    TWO_TERM_DIFF_FILTER,
    TWO_TERM_AVG_FILTER,
    THREE_TERM_AVG_FILTER,
    CENTRAL_DIFF_FILTER,
    RECURSIVR_FILTER,
    MOOG_FILTER
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
        float f_buffer_size = 0;
        int f_sample_rate = 0;
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
        void setSampleRate(int new_sample_rate);
        virtual AudioBuffer<float> applyFilter(int channel) = 0;
        virtual void applyFilter(AudioBuffer<float>& buffer, int channel) = 0;
        AudioBuffer<float> applyFilter(int channel);
};



class TwoTermDifferenceFilter : public Filter
{
    public:
        TwoTermDifferenceFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : Filter(buffer, new_logic) {}
        virtual ~TwoTermDifferenceFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

class TwoTermAverageFilter : public Filter
{
    public:
        TwoTermAverageFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : Filter(buffer, new_logic) {}
        virtual ~TwoTermAverageFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

class ThreeTermAverageFilter : public Filter
{
    public:
        ThreeTermAverageFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : Filter(buffer, new_logic) {}
        virtual ~ThreeTermAverageFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

class CenteralDifferenceFilter : public Filter
{
    public:
        CenteralDifferenceFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : Filter(buffer, new_logic) {}
        virtual ~CenteralDifferenceFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};


class RecursiveFilter : public Filter
{
    public:
        RecursiveFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : Filter(buffer, new_logic) {}
        virtual ~RecursiveFilter() = default;
        AudioBuffer<float> applyFilter(int channel) override;
};

class MoogFilter : public Filter
{
    private:
        float frequency;
        float g;
        float resonance;
        float drive;

        float y_a;
        float y_b;
        float y_c;
        float y_d;
        float y_d_1;
    public:
        MoogFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : Filter(buffer, new_logic) {}
        virtual ~MoogFilter() = default;
        void applyFilter(AudioBuffer<float>& buffer, int channel) override;
        float getFrequency();
        float getResonance();
        float getDrive();

        void setFrequency(float new_freq);
        void setResonance(float new_res);
        void setDrive(float d);
};

#endif