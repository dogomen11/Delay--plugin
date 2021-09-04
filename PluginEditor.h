/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyDelay.h"

using namespace juce;

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener,
                                        public juce::Button::Listener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;
    void juce::Slider::Listener::sliderValueChanged(Slider* slider) override;
    void juce::Button::Listener::buttonClicked(Button*) override;
    void initiateComponents(NewProjectAudioProcessor&);
    void printComponents();
    
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> slider_attach;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:

    juce::Slider m_input_gain;
    juce::Label m_input_gain_label;
    juce::Slider m_output_gain;
    juce::Label m_output_gain_label;
    //******************************
    juce::Slider m_volume_dial_1;
    juce::Slider m_volume_dial_2;
    juce::Slider m_volume_dial_3;
    juce::Slider m_volume_dial_4;
    juce::Slider m_volume_dial_5;
    juce::Slider m_volume_dial_6;
    juce::Slider m_volume_dial_7;
    juce::Slider m_volume_dial_8;
    juce::Slider m_volume_dial_9;
    juce::Slider m_volume_dial_10;
    juce::Slider m_volume_dial_11;
    juce::Slider m_volume_dial_12;
    juce::Slider m_volume_dial_13;
    juce::Slider m_volume_dial_14;
    juce::Slider m_volume_dial_15;
    juce::Slider m_volume_dial_16;
    //********************************
    juce::Slider m_pan_dial_1;
    juce::Slider m_pan_dial_2;
    juce::Slider m_pan_dial_3;
    juce::Slider m_pan_dial_4;
    juce::Slider m_pan_dial_5;
    juce::Slider m_pan_dial_6;
    juce::Slider m_pan_dial_7;
    juce::Slider m_pan_dial_8;
    juce::Slider m_pan_dial_9;
    juce::Slider m_pan_dial_10;
    juce::Slider m_pan_dial_11;
    juce::Slider m_pan_dial_12;
    juce::Slider m_pan_dial_13;
    juce::Slider m_pan_dial_14;
    juce::Slider m_pan_dial_15;
    juce::Slider m_pan_dial_16;
    //**********************************
    juce::ToggleButton m_on_off_button_1;
    juce::ToggleButton m_on_off_button_2;
    juce::ToggleButton m_on_off_button_3;
    juce::ToggleButton m_on_off_button_4;
    juce::ToggleButton m_on_off_button_5;
    juce::ToggleButton m_on_off_button_6;
    juce::ToggleButton m_on_off_button_7;
    juce::ToggleButton m_on_off_button_8;
    juce::ToggleButton m_on_off_button_9;
    juce::ToggleButton m_on_off_button_10;
    juce::ToggleButton m_on_off_button_11;
    juce::ToggleButton m_on_off_button_12;
    juce::ToggleButton m_on_off_button_13;
    juce::ToggleButton m_on_off_button_14;
    juce::ToggleButton m_on_off_button_15;
    juce::ToggleButton m_on_off_button_16;


    //************************************
    juce::Label m_volume_dials_label;
    juce::Label m_pan_dials_label;
    juce::Label m_on_off_buttons_label;
    //************************************
    juce::Slider m_delay_time;
    juce::Label m_delay_time_label;
    juce::Slider m_delay_mix;
    juce::Label m_delay_mix_label;
    //************************************

    //************************************

    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
