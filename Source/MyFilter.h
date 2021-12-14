/*
  ==============================================================================

    MyFilter.h
    Created: 24 Aug 2021 12:47:44pm
    Author:  dogom

  ==============================================================================
*/

#ifndef MYFILTER
#define MYFILTER

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

/*
class MyFilter
{
protected:
    float f_buffer_size = 0;
    int f_sample_rate = 0;
    float coefficients[5];
    void setLowPass();
    void setHighPass();
    void setBandPass();
public:
    MyFilter(filter_logic new_logic);
    ~MyFilter() = default;
    void setFilterType(filter_type new_type);
    void setSampleRate(int new_sample_rate);
    void applyFilter(float* buffer, int channel);
    AudioBuffer<float> applyFilter(int channel);
};



class TwoTermDifferenceFilter : public MyFilter
{
    public:
        TwoTermDifferenceFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : MyFilter(buffer, new_logic) {}
        virtual ~TwoTermDifferenceFilter() = default;
        AudioBuffer<float> applyFilter(int channel);
};

class TwoTermAverageFilter : public MyFilter
{
    public:
        TwoTermAverageFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : MyFilter(buffer, new_logic) {}
        virtual ~TwoTermAverageFilter() = default;
        AudioBuffer<float> applyFilter(int channel);
};

class ThreeTermAverageFilter : public MyFilter
{
    public:
        ThreeTermAverageFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : MyFilter(buffer, new_logic) {}
        virtual ~ThreeTermAverageFilter() = default;
        AudioBuffer<float> applyFilter(int channel);
};

class CenteralDifferenceFilter : public MyFilter
{
    public:
        CenteralDifferenceFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : MyFilter(buffer, new_logic) {}
        virtual ~CenteralDifferenceFilter() = default;
        AudioBuffer<float> applyFilter(int channel);
};


class RecursiveFilter : public MyFilter
{
    public:
        RecursiveFilter(AudioBuffer<float>& buffer, filter_logic new_logic) : MyFilter(buffer, new_logic) {}
        virtual ~RecursiveFilter() = default;
        AudioBuffer<float> applyFilter(int channel);
};

*/
class MoogFilter
{
private:
    float frequency;
    float g;
    float resonance;
    float drive;
    int f_sample_rate;

    float y_a;
    float y_b;
    float y_c;
    float y_d;
    float y_d_1;
public:
    MoogFilter();
    ~MoogFilter() = default;
    void applyFilter(float* buffer);
    float getFrequency();
    float getResonance();
    float getDrive();

    void setSampleRate(int new_sample_rate);
    void setFrequency(float new_freq);
    void setResonance(float new_res);
    void setDrive(float d);
};

#endif