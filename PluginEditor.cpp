/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &m_outgain)
    {
        audioProcessor.m_gaim = m_outgain.getValue();
    }
}

void NewProjectAudioProcessorEditor::buttonClicked(Button*)
{
    DBG ("clicked");
}

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(1200, 800);

    slider_attach = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, GAIN_ID, m_outgain);
    initiateComponents(p);

}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}


void NewProjectAudioProcessorEditor::initiateComponents(NewProjectAudioProcessor& p)
{

    addAndMakeVisible(m_volume_dials_label);
    m_volume_dials_label.setText("vol", juce::dontSendNotification);
    addAndMakeVisible(m_pan_dials_label);
    m_pan_dials_label.setText("pan", juce::dontSendNotification);
    addAndMakeVisible(m_on_off_buttons_label);
    m_on_off_buttons_label.setText("on/off", juce::dontSendNotification);
    //*****************************************************************************
    addAndMakeVisible(m_outgain);
    m_outgain.setRange(-12.0f, 12.0f, 0.01f);
    m_outgain.setValue(0.0f);
    m_outgain.setTextValueSuffix("db");
    m_outgain.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_outgain.addListener(this);
    addAndMakeVisible(m_outgain_label);
    m_outgain_label.setText("gain", juce::dontSendNotification);

    addAndMakeVisible(m_delay_time);
    m_delay_time.setRange(0.0f, 1000.0f, 1.0f);
    m_delay_time.setValue(0.0f);
    m_delay_time.setTextValueSuffix("ms");
    m_delay_time.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_delay_time.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_delay_time.addListener(this);
    addAndMakeVisible(m_delay_time_label);
    m_delay_time_label.setText("time", juce::dontSendNotification);

    addAndMakeVisible(m_delay_mix);
    m_delay_mix.setRange(0.0f, 100.0f, 0.01f);
    m_delay_mix.setValue(50.0f);
    m_delay_mix.setTextValueSuffix("%");
    m_delay_mix.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_delay_mix.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_delay_mix.addListener(this);
    addAndMakeVisible(m_delay_mix_label);
    m_delay_mix_label.setText("mix", juce::dontSendNotification);
    //*****************************************************************************
    addAndMakeVisible(m_volume_dial_1);
    m_volume_dial_1.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_1.setValue(6.0f);
    m_volume_dial_1.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_1.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_1.addListener(this);

    addAndMakeVisible(m_volume_dial_2);
    m_volume_dial_2.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_2.setValue(6.0f);
    m_volume_dial_2.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_2.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_2.addListener(this);

    addAndMakeVisible(m_volume_dial_3);
    m_volume_dial_3.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_3.setValue(6.0f);
    m_volume_dial_3.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_3.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_3.addListener(this);

    addAndMakeVisible(m_volume_dial_4);
    m_volume_dial_4.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_4.setValue(6.0f);
    m_volume_dial_4.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_4.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_4.addListener(this);

    addAndMakeVisible(m_volume_dial_5);
    m_volume_dial_5.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_5.setValue(6.0f);
    m_volume_dial_5.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_5.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_5.addListener(this);

    addAndMakeVisible(m_volume_dial_6);
    m_volume_dial_6.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_6.setValue(6.0f);
    m_volume_dial_6.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_6.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_6.addListener(this);

    addAndMakeVisible(m_volume_dial_7);
    m_volume_dial_7.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_7.setValue(6.0f);
    m_volume_dial_7.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_7.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_7.addListener(this);

    addAndMakeVisible(m_volume_dial_8);
    m_volume_dial_8.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_8.setValue(6.0f);
    m_volume_dial_8.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_8.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_8.addListener(this);

    addAndMakeVisible(m_volume_dial_9);
    m_volume_dial_9.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_9.setValue(6.0f);
    m_volume_dial_9.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_9.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_9.addListener(this);

    addAndMakeVisible(m_volume_dial_10);
    m_volume_dial_10.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_10.setValue(6.0f);
    m_volume_dial_10.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_10.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_10.addListener(this);

    addAndMakeVisible(m_volume_dial_11);
    m_volume_dial_11.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_11.setValue(6.0f);
    m_volume_dial_11.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_11.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_11.addListener(this);

    addAndMakeVisible(m_volume_dial_12);
    m_volume_dial_12.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_12.setValue(6.0f);
    m_volume_dial_12.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_12.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_12.addListener(this);

    addAndMakeVisible(m_volume_dial_13);
    m_volume_dial_13.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_13.setValue(6.0f);
    m_volume_dial_13.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_13.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_13.addListener(this);

    addAndMakeVisible(m_volume_dial_14);
    m_volume_dial_14.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_14.setValue(6.0f);
    m_volume_dial_14.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_14.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_14.addListener(this);

    addAndMakeVisible(m_volume_dial_15);
    m_volume_dial_15.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_15.setValue(6.0f);
    m_volume_dial_15.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_15.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_15.addListener(this);

    addAndMakeVisible(m_volume_dial_16);
    m_volume_dial_16.setRange(-12.0f, 0.0f, 0.01f);
    m_volume_dial_16.setValue(6.0f);
    m_volume_dial_16.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_volume_dial_16.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_volume_dial_16.addListener(this);
    //*****************************************************************************
    addAndMakeVisible(m_pan_dial_1);
    m_pan_dial_1.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_1.setValue(0.0f);
    m_pan_dial_1.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_1.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_1.addListener(this);

    addAndMakeVisible(m_pan_dial_2);
    m_pan_dial_2.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_2.setValue(0.0f);
    m_pan_dial_2.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_2.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_2.addListener(this);

    addAndMakeVisible(m_pan_dial_3);
    m_pan_dial_3.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_3.setValue(0.0f);
    m_pan_dial_3.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_3.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_3.addListener(this);

    addAndMakeVisible(m_pan_dial_4);
    m_pan_dial_4.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_4.setValue(0.0f);
    m_pan_dial_4.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_4.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_4.addListener(this);

    addAndMakeVisible(m_pan_dial_5);
    m_pan_dial_5.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_5.setValue(0.0f);
    m_pan_dial_5.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_5.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_5.addListener(this);

    addAndMakeVisible(m_pan_dial_6);
    m_pan_dial_6.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_6.setValue(0.0f);
    m_pan_dial_6.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_6.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_6.addListener(this);

    addAndMakeVisible(m_pan_dial_7);
    m_pan_dial_7.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_7.setValue(0.0f);
    m_pan_dial_7.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_7.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_7.addListener(this);

    addAndMakeVisible(m_pan_dial_8);
    m_pan_dial_8.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_8.setValue(0.0f);
    m_pan_dial_8.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_8.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_8.addListener(this);

    addAndMakeVisible(m_pan_dial_9);
    m_pan_dial_9.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_9.setValue(0.0f);
    m_pan_dial_9.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_9.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_9.addListener(this);

    addAndMakeVisible(m_pan_dial_10);
    m_pan_dial_10.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_10.setValue(0.0f);
    m_pan_dial_10.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_10.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_10.addListener(this);

    addAndMakeVisible(m_pan_dial_11);
    m_pan_dial_11.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_11.setValue(0.0f);
    m_pan_dial_11.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_11.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_11.addListener(this);

    addAndMakeVisible(m_pan_dial_12);
    m_pan_dial_12.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_12.setValue(0.0f);
    m_pan_dial_12.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_12.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_12.addListener(this);

    addAndMakeVisible(m_pan_dial_13);
    m_pan_dial_13.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_13.setValue(0.0f);
    m_pan_dial_13.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_13.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_13.addListener(this);

    addAndMakeVisible(m_pan_dial_14);
    m_pan_dial_14.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_14.setValue(0.0f);
    m_pan_dial_14.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_14.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_14.addListener(this);

    addAndMakeVisible(m_pan_dial_15);
    m_pan_dial_15.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_15.setValue(0.0f);
    m_pan_dial_15.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_15.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_15.addListener(this);

    addAndMakeVisible(m_pan_dial_16);
    m_pan_dial_16.setRange(-50.0f, 50.0f, 0.01f);
    m_pan_dial_16.setValue(0.0f);
    m_pan_dial_16.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    m_pan_dial_16.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    m_pan_dial_16.addListener(this);
    //*****************************************************************************
    addAndMakeVisible(m_on_off_button_1);
    m_on_off_button_1.addListener(this);

    addAndMakeVisible(m_on_off_button_2);
    m_on_off_button_2.addListener(this);

    addAndMakeVisible(m_on_off_button_3);
    m_on_off_button_3.addListener(this);

    addAndMakeVisible(m_on_off_button_4);
    m_on_off_button_4.addListener(this);

    addAndMakeVisible(m_on_off_button_5);
    m_on_off_button_5.addListener(this);

    addAndMakeVisible(m_on_off_button_6);
    m_on_off_button_6.addListener(this);

    addAndMakeVisible(m_on_off_button_7);
    m_on_off_button_7.addListener(this);

    addAndMakeVisible(m_on_off_button_8);
    m_on_off_button_8.addListener(this);

    addAndMakeVisible(m_on_off_button_9);
    m_on_off_button_9.addListener(this);

    addAndMakeVisible(m_on_off_button_10);
    m_on_off_button_10.addListener(this);

    addAndMakeVisible(m_on_off_button_11);
    m_on_off_button_11.addListener(this);

    addAndMakeVisible(m_on_off_button_12);
    m_on_off_button_12.addListener(this);

    addAndMakeVisible(m_on_off_button_13);
    m_on_off_button_13.addListener(this);

    addAndMakeVisible(m_on_off_button_14);
    m_on_off_button_14.addListener(this);

    addAndMakeVisible(m_on_off_button_15);
    m_on_off_button_15.addListener(this);

    addAndMakeVisible(m_on_off_button_16);
    m_on_off_button_16.addListener(this);
    //*****************************************************************************

}


void NewProjectAudioProcessorEditor::printComponents()
{
    int dials_horizontal_distance = 66;
    int dials_distance_from_edeg = 68;
    int size_of_dial = 62;

    m_outgain.setBounds(1050, 0, 150, 150);
    m_outgain_label.setBounds(1145, 80, 80, 50);
    m_pan_dials_label.setBounds(15, 510, 80, 50);
    m_volume_dials_label.setBounds(15, 580, 80, 50);
    m_on_off_buttons_label.setBounds(15, 650, 80, 50);
    m_delay_time.setBounds(200, 700, 90, 90);
    m_delay_time_label.setBounds(180, 735, 90, 30);
    m_delay_mix.setBounds(400, 700, 90, 90);
    m_delay_mix_label.setBounds(270, 735, 90, 30);

    m_volume_dial_1.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 0, 580, size_of_dial, size_of_dial);
    m_volume_dial_2.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 1, 580, size_of_dial, size_of_dial);
    m_volume_dial_3.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 2, 580, size_of_dial, size_of_dial);
    m_volume_dial_4.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 3, 580, size_of_dial, size_of_dial);
    m_volume_dial_5.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 4, 580, size_of_dial, size_of_dial);
    m_volume_dial_6.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 5, 580, size_of_dial, size_of_dial);
    m_volume_dial_7.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 6, 580, size_of_dial, size_of_dial);
    m_volume_dial_8.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 7, 580, size_of_dial, size_of_dial);
    m_volume_dial_9.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 8, 580, size_of_dial, size_of_dial);
    m_volume_dial_10.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 9, 580, size_of_dial, size_of_dial);
    m_volume_dial_11.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 10, 580, size_of_dial, size_of_dial);
    m_volume_dial_12.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 11, 580, size_of_dial, size_of_dial);
    m_volume_dial_13.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 12, 580, size_of_dial, size_of_dial);
    m_volume_dial_14.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 13, 580, size_of_dial, size_of_dial);
    m_volume_dial_15.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 14, 580, size_of_dial, size_of_dial);
    m_volume_dial_16.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 15, 580, size_of_dial, size_of_dial);

    m_pan_dial_1.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 0, 510, size_of_dial, size_of_dial);
    m_pan_dial_2.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 1, 510, size_of_dial, size_of_dial);
    m_pan_dial_3.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 2, 510, size_of_dial, size_of_dial);
    m_pan_dial_4.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 3, 510, size_of_dial, size_of_dial);
    m_pan_dial_5.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 4, 510, size_of_dial, size_of_dial);
    m_pan_dial_6.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 5, 510, size_of_dial, size_of_dial);
    m_pan_dial_7.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 6, 510, size_of_dial, size_of_dial);
    m_pan_dial_8.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 7, 510, size_of_dial, size_of_dial);
    m_pan_dial_9.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 8, 510, size_of_dial, size_of_dial);
    m_pan_dial_10.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 9, 510, size_of_dial, size_of_dial);
    m_pan_dial_11.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 10, 510, size_of_dial, size_of_dial);
    m_pan_dial_12.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 11, 510, size_of_dial, size_of_dial);
    m_pan_dial_13.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 12, 510, size_of_dial, size_of_dial);
    m_pan_dial_14.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 13, 510, size_of_dial, size_of_dial);
    m_pan_dial_15.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 14, 510, size_of_dial, size_of_dial);
    m_pan_dial_16.setBounds(dials_distance_from_edeg + dials_horizontal_distance * 15, 510, size_of_dial, size_of_dial);

    m_on_off_button_1.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 0, 650, size_of_dial, size_of_dial);
    m_on_off_button_2.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 1, 650, size_of_dial, size_of_dial);
    m_on_off_button_3.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 2, 650, size_of_dial, size_of_dial);
    m_on_off_button_4.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 3, 650, size_of_dial, size_of_dial);
    m_on_off_button_5.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 4, 650, size_of_dial, size_of_dial);
    m_on_off_button_6.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 5, 650, size_of_dial, size_of_dial);
    m_on_off_button_7.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 6, 650, size_of_dial, size_of_dial);
    m_on_off_button_8.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 7, 650, size_of_dial, size_of_dial);
    m_on_off_button_9.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 8, 650, size_of_dial, size_of_dial);
    m_on_off_button_10.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 9, 650, size_of_dial, size_of_dial);
    m_on_off_button_11.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 10, 650, size_of_dial, size_of_dial);
    m_on_off_button_12.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 11, 650, size_of_dial, size_of_dial);
    m_on_off_button_13.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 12, 650, size_of_dial, size_of_dial);
    m_on_off_button_14.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 13, 650, size_of_dial, size_of_dial);
    m_on_off_button_15.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 14, 650, size_of_dial, size_of_dial);
    m_on_off_button_16.setBounds(16 + dials_distance_from_edeg + dials_horizontal_distance * 15, 650, size_of_dial, size_of_dial);

}



//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.setColour(Colours::maroon);
    Rectangle<float> area(5, 5, 1195, 795);
    g.drawRoundedRectangle(area, 20.0f, 3.0f);
}

void NewProjectAudioProcessorEditor::resized()
{

    printComponents();

}