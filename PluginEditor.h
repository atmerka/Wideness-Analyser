/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Meter1.h"
#include "StyleXo.h"
//==============================================================================
/**
*/
class WidenessAnalyserAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Timer, public Slider::Listener
{
public:
    WidenessAnalyserAudioProcessorEditor (WidenessAnalyserAudioProcessor&);
    ~WidenessAnalyserAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    StyleXo styleXo;
    
    Meter1 m1;
    ToggleButton panButton;
    Slider avgLength;
    
    //define functions
    void timerCallback() override;
    void sliderValueChanged(Slider* slider) override;
    
    
    //Slider frequency;
    
    double f;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WidenessAnalyserAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WidenessAnalyserAudioProcessorEditor)
};
