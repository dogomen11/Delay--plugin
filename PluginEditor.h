/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                                                   public juce::Slider::Listener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;
    void juce::Slider::Listener::sliderValueChanged(juce::Slider* slider) override;
    
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider mMakeUpGain;
    juce::Label mMakeUpGainLabel;

    juce::Slider mThreshhold;
    juce::Label mThreshholdLabel;

    juce::Slider mAttack;
    juce::Label mAttackLabel;

    juce::Slider mRealese;
    juce::Label mRealeseLabel;

    juce::Slider mRatio;
    juce::Label mRatioLabel;

    NewProjectAudioProcessor& audioProcessor;

    //like smart_ptr
    juce::ScopedPointer <juce::AudioProcessorValueTreeState::SliderAttachment> slider_attach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
