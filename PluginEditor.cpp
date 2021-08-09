/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mMakeUpGain)
    {
        audioProcessor.mGain = mMakeUpGain.getValue();
    }
}
//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    slider_attach = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, GAIN_ID, mMakeUpGain);

    setSize (1000, 600);

    addAndMakeVisible(mMakeUpGain);
    mMakeUpGain.setRange(-12.0f, 12.0f, 0.01f);
    mMakeUpGain.setValue(0.0f);
    mMakeUpGain.setTextValueSuffix("db");
    mMakeUpGain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(mMakeUpGainLabel);
    mMakeUpGain.addListener(this);
    mMakeUpGainLabel.setText("gain", juce::dontSendNotification);

    
    addAndMakeVisible(mThreshhold);
    mThreshhold.setRange(-12.0f, 0.0f, 0.01f);
    mThreshhold.setValue(0.0f);
    mThreshhold.setTextValueSuffix("db");
    mThreshhold.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(mThreshholdLabel);
    mThreshholdLabel.setText("threshhold", juce::dontSendNotification);

    

    addAndMakeVisible(mAttack);
    mAttack.setRange(0.01f, 1.0f, 0.001f);
    mAttack.setValue(0.04f);
    mAttack.setTextValueSuffix("ms");
    mAttack.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(mAttackLabel);
    mAttackLabel.setText("attack", juce::dontSendNotification);


    addAndMakeVisible(mRealese);
    mRealese.setRange(0.01f, 4.0f, 0.001f);
    mRealese.setValue(0.0f);
    mRealese.setTextValueSuffix("ms");
    mRealese.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(mRealeseLabel);
    mRealeseLabel.setText("realese", juce::dontSendNotification);

    addAndMakeVisible(mRatio);
    mRatio.setRange(-12.0f, 12.0f, 0.01f);
    mRatio.setValue(0.0f);
    mRatio.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    addAndMakeVisible(mRatioLabel);
    mRatioLabel.setText("ratio", juce::dontSendNotification);


    filter_cutoff_value = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, "cutoff", mThreshhold);
    mThreshhold.setSkewFactorFromMidPoint(1000.0f);

    filter_resonance_value = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, "resonance", mAttack);
    

}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.setColour(Colours::maroon);
    Rectangle<float> area(5, 5, 990, 590);
    g.drawRoundedRectangle(area, 20.0f, 3.0f);
}

void NewProjectAudioProcessorEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(8);
    unsigned mDistansBetweenDials = 190;

    mMakeUpGain.setBounds(45,  420, 150, 180);
    mMakeUpGainLabel.setBounds(140, 520, 80, 50);

    mThreshhold.setBounds(45+(mDistansBetweenDials*1), 420, 150, 180);
    mThreshholdLabel.setBounds(140 + (mDistansBetweenDials * 1) - 13, 520, 80, 50);

    mAttack.setBounds(45 + (mDistansBetweenDials * 2), 420, 150, 180);
    mAttackLabel.setBounds(140 + (mDistansBetweenDials * 2) - 3, 520, 80, 50);

    mRealese.setBounds(45 + (mDistansBetweenDials * 3), 420, 150, 180);
    mRealeseLabel.setBounds(140 + (mDistansBetweenDials * 3) - 7, 520, 80, 50);

    mRatio.setBounds(45 + (mDistansBetweenDials * 4), 420, 150, 180);
    mRatioLabel.setBounds(140 + (mDistansBetweenDials * 4) , 520, 80, 50);

}
