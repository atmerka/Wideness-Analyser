/*
  ==============================================================================

    BasicStyle.h
    Created: 6 Jul 2022 3:46:46pm
    Author:  trissy the sissy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class StyleXo    : public LookAndFeel_V4
{
public:

    /** Draws the contents of a standard ToggleButton. */
     void drawToggleButton (Graphics&, ToggleButton&,
                                   bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
 
    
    void drawLinearSlider (Graphics& ,
                                      int x, int y, int width, int height,
                                      float sliderPos,
                                      float minSliderPos,
                                      float maxSliderPos,
                                      const Slider::SliderStyle,
                           Slider& ) override;
    
    
    void drawRotarySlider (Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider&) override;
    
//    void drawLabel (Graphics&, Label& ) override;
//
//
//    void drawButtonText (juce::Graphics&, juce::TextButton& , bool isMouseOverButton, bool isButtonDown) override;
    
private:
    
};
