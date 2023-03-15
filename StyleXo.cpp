/*
  ==============================================================================

    BasicStyle.cpp
    Created: 6 Jul 2022 3:46:46pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "StyleXo.h"

//==============================================================================
//BasicStyle::


/** Draws the contents of a standard ToggleButton. */
void StyleXo::drawToggleButton (Graphics& g, ToggleButton& t,
                       bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    
    if(shouldDrawButtonAsDown)
    {
        g.setColour(Colours::grey);
        
        //    g.drawRect(t.getX(), t.getY(), t.getWidth(), t.getHeight());
        g.fillRect(t.getLocalBounds() );
        
    }
    else {
        
        if(t.getToggleState())
        {
            g.setColour(Colours::lightblue.withAlpha(0.5f));
        }
        else
        {
            g.setColour(Colours::lightgrey.withAlpha(0.3f));
        }
        g.fillRect(t.getLocalBounds());
        
        g.setColour(Colours::black);
    //    g.drawRect(t.getX(), t.getY(), t.getWidth(), t.getHeight());
        g.drawRect(t.getLocalBounds());
        
        
        g.setColour(t.findColour(0x1006502));
        
        g.drawText(t.getButtonText(), t.getLocalBounds(), Justification::centred);
    }
}



void StyleXo::drawLinearSlider (Graphics& g,
                                  int x, int y, int width, int height,
                                  float sliderPos,
                                  float minSliderPos,
                                  float maxSliderPos,
                                  const Slider::SliderStyle,
                                  Slider& s)
{
    
    
    if(s.getSliderStyle() == Slider::LinearVertical)
    {
        
        
        
        float ratio = s.getValue()/s.getMaximum();
        int numDrawn = int(20 *ratio);
        
        for(int i=0; i<20;i++)
        {
         
            float ratio = i/20.0f;
            
            g.setColour(Colours::lightblue);
            
            g.fillRect(width/2-15.0f, height-ratio*height, 30.0f, 10.0f);
            
//            g.drawRect(0, height-ratio*height, 10, 10);
            
            
            
            
            if(i>= numDrawn) break;
        }
        
        
        
        //DRAWING NUMBERS
        
        juce::String suffix = s.getTextValueSuffix();
        
        if(s.getTextBoxPosition() == Slider::TextBoxLeft)
        {
            g.setColour(Colours::black.withAlpha(0.4f));
            float gain = s.getValue();
            gain = gain;
            gain = juce::Decibels::gainToDecibels(gain);
            
            gain = std::ceil(gain*100)/100;
            g.drawText((String)gain+ suffix, 0, 0, width, height, Justification::centred);
        }
        
//        if(s.getTextBoxPosition() == Slider::TextBoxRight)
//        {
//            float perc = s.getValue()*100;
//
//            g.drawText((String)perc + suffix, 0, 0, width, height, Justification::centred);
//        }
        
    }
    else if(s.getSliderStyle() == Slider::LinearHorizontal)
    {
    
        //draw outline
        g.setColour(Colours::black);
        g.drawRect(x, y, width, height);
        
        g.setColour(Colours::white);
        g.drawText(s.getTextFromValue(s.getValue()), s.getLocalBounds(), Justification::centred);
        
        
        g.setColour(Colours::grey.withAlpha(0.4f));
        
        g.fillRect(x, y,int(sliderPos-12) , height);
        
        
        
        g.setColour(Colours::lightblue.withAlpha(0.6f));
        g.drawRect(x, y,int(sliderPos-12) , height);
        
        
        if(s.getValue()> (s.getMaximum()/2))
        {
    //        g.drawText(s.getTextFromValue(maxSliderPos), s.getLocalBounds(), Justification::centred);
            
            g.setColour(Colours::white);
            g.drawText((s.getTextFromValue(s.getValue())), s.getLocalBounds(), Justification::centred);
        }
    }
}
    
    
void StyleXo::drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                                       float sliderPosProportional, float rotaryStartAngle,
                                       float rotaryEndAngle, Slider& s)
{
    
    
    //draw a circle of blue balls
    
    //convert the value of the slider to an integer 1- 20
    
    float ratio = s.getValue()/s.getMaximum();
    int numDrawn = int(20 *ratio);
    
    float pitwo = 2 * 3.1415;
    float r = width/2-12;
    //draw each ball at the right place until we get to the ratio
    for(int i=0; i< 20;i++)
    {
        
        //x is the middle
        //cos0 = a/h
        //sin0 = o /h
        
        //adjacent = cos0 * R
        //opposite = sin0 * R
        
        //radius = getWidth/2
        
        float adjacent = cos((i/20.0f)*pitwo)*r;
        float opposite = sin((i/20.0f)*pitwo)*r;
        
        //x is middle minus distance to x
        //y is midd +distance to y
        g.setColour(Colours::aliceblue);
        g.fillEllipse(width/2-adjacent, height/2-opposite, 9.0f, 9.0f);
        
        
        
        g.setColour(Colours::lightblue);
        g.drawEllipse(width/2-adjacent, height/2-opposite, 9.0f, 9.0f, 2.0f);
        
        
        if(i>(numDrawn-2)) break;
        
        
    }
    
    juce::String suffix = s.getTextValueSuffix();

    
    
    if(s.getTextBoxPosition() == Slider::TextBoxLeft)
    {
        float gain = s.getValue();
        gain = 4* gain;
        gain = std::pow(10, gain);
        gain = juce::Decibels::gainToDecibels(gain);
        g.drawText((String)gain+ suffix, 0, 0, width, height, Justification::centred);
    }
    
    if(s.getTextBoxPosition() == Slider::TextBoxRight)
    {
        float perc = s.getValue()*100;

        g.drawText((String)perc + suffix, 0, 0, width, height, Justification::centred);
    }
    
//    g.drawEllipse(cos((0/20)*pitwo)*r, sin((0/20)*pitwo)*r, 10.0f, 10.0f, 2.0f);
//    g.drawEllipse(cos((1/20)*pitwo)*r, sin((1/20)*pitwo)*r, 10.0f, 10.0f, 2.0f);
        
    
    
}

//void BasicStyle::drawLabel (Graphics& g, Label& label)
//{
//
//    label.getText();
//    label.getHeight();
//    label.getX();
//    label.getY();
//
//
//    g.setColour(Colours::black);
//    g.drawText(label.getText(), label.getX(), label.getY(), label.getWidth(), label.getHeight(), Justification::centred);
//
//
//}


//void BasicStyle::drawButtonText (juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown)
//{
//
//
//
//
//    auto font = getTextButtonFont (button, button.getHeight());
//    g.setFont (font);
//    g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId
//                                                            : juce::TextButton::textColourOffId)
//                       .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
//
//    auto yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
//    auto cornerSize = juce::jmin (button.getHeight(), button.getWidth()) / 2;
//
//    auto fontHeight = juce::roundToInt (font.getHeight() * 0.6f);
//    auto leftIndent  = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft()  ? 4 : 2));
//    auto rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
//    auto textWidth = button.getWidth() - leftIndent - rightIndent;
//
//    if (textWidth > 0)
//        g.drawFittedText (button.getButtonText(),
//                          leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
//                          juce::Justification::centred, 2);
//}
