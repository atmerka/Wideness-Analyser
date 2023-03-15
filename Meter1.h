/*
  ==============================================================================

    Meter1.h
    Created: 28 Dec 2020 4:16:32pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class Meter1    : public Component
{
public:
    Meter1(WidenessAnalyserAudioProcessor&);
    ~Meter1();

    void paint (Graphics&) override;
    void resized() override;
    
    
    //getting values
    float getAndScaleValueL(int i);

    float getAndScaleValueR(int i);
    
    float getAndScaleRMS();

    
private:
    
    //to draw vis
    double freq;
    Path spectL, spectR;
    int count = 0;
    float skewedProportionX;
    
    
    
    
    WidenessAnalyserAudioProcessor& processor;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter1)
};
