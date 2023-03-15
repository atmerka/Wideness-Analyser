/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WidenessAnalyserAudioProcessorEditor::WidenessAnalyserAudioProcessorEditor (WidenessAnalyserAudioProcessor& p)
    : AudioProcessorEditor (&p), m1(p), processor (p)
{
    //set timercallback to start
    startTimerHz(30);
    
    //set plug in to resizable
    AudioProcessorEditor::setResizable(true, true);
    AudioProcessorEditor::setResizeLimits(100, 100, 1600, 1000);
    setSize(600, 600);
    addAndMakeVisible(m1);
    
    //drawing avg slider
    addAndMakeVisible(avgLength);
    avgLength.setRange(1, 501,1);
    avgLength.setSliderStyle(Slider::LinearHorizontal);
    //avgLength.setValue(40);
    avgLength.setSkewFactor(0.6f, false);
    avgLength.addListener(this);
    avgLength.setColour(0x1001200, Colour(35, 196, 204));
    avgLength.setColour(0x1001300, Colour(35, 196, 204));
    avgLength.setColour(0x1001310, Colour(35, 196, 204));
    avgLength.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    avgLength.setValue(30);
    
    avgLength.setLookAndFeel(&styleXo);
    
    
    //drawing pan button
    panButton.setToggleState(false, NotificationType::dontSendNotification);
    panButton.setBounds(getWidth()*0.1, 0.0, getWidth()*0.1, getHeight()*0.1);
   // panButton.setColour(<#int colourID#>, <#juce::Colour newColour#>)
    panButton.setLookAndFeel(&styleXo);
    panButton.setButtonText("INCL. PAN");
    addAndMakeVisible(panButton);
    
}

WidenessAnalyserAudioProcessorEditor::~WidenessAnalyserAudioProcessorEditor()
{
}

//==============================================================================
void WidenessAnalyserAudioProcessorEditor::paint (Graphics& g)
{
    //fill background Dark Blue
    g.fillAll(Colours::black.withAlpha(0.4f));
    
    //change colour
    g.setColour(Colours::lightblue.withAlpha(0.6f));
    
    g.setFont(20.0f);
    //draw channel labels at top
    g.drawText("WIDENESS ANALYSER", getWidth()*.01, getHeight()*.01, getWidth(), getHeight() / 10, Justification::centred, true);
    
    
    //tidy this up to buttonvalue changed function
    if(panButton.getToggleState())
    {
        processor.panDraw = true;
    }
    if(panButton.getToggleState() == false)
    {
        processor.panDraw = false;
    }
    
}

void WidenessAnalyserAudioProcessorEditor::resized()
{
    //frequency.setBounds(getWidth() / 3, getHeight()*0.1, getWidth() / 3, getHeight()*.9);
    
    
    m1.setBounds(0.0, 0.1*getHeight(), getWidth(), getHeight()*.9);
    avgLength.setBounds(getWidth()*0.75, 0.0, getWidth()*0.25, getHeight()*0.1);
    panButton.setBounds(getWidth()*0.1, 0.0, getWidth()*0.1, getHeight()*0.1);
}

void WidenessAnalyserAudioProcessorEditor::timerCallback()
{
    //repaint at a fast rate
    repaint();
}

void WidenessAnalyserAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    processor.nextAvg = avgLength.getValue();
    
}
