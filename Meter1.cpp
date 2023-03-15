/*
  ==============================================================================

    Meter1.cpp
    Created: 28 Dec 2020 4:16:32pm
    Author:  trissy the sissy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Meter1.h"

//==============================================================================
Meter1::Meter1(WidenessAnalyserAudioProcessor& p) : processor(p)

{
    

}

Meter1::~Meter1()
{
}

void Meter1::paint (Graphics& g)
{
    
    g.fillAll (Colours::black.withAlpha(0.4f));
    g.fillAll (Colours::darkgrey.withAlpha(0.4f));
    
    g.setColour(Colours::black);
    g.drawRect(0,0,getWidth(),getHeight());

    //get size of FFT array from the processor
    //deems number of bins, frequency etc
    int fftSize = processor.getFFTSizeL();
    
    
    //===============================DRAWING FREQ LINES=============================
    for ( int i = 0 ; i < fftSize/2 ;i++)
    {
        skewedProportionX =   std::exp (std::log ((float) i / (float) (fftSize/2)) * 0.2f);
        
        //drawing frequency lines
        
        //frequency = 10.91 *i
        // f = 100 , i = 9||f = 500, i = 45.8||f = 1000 , i = 91.5||f = 5000, i = 458|| f = 10000, i = 917|| f = 20000, i = 1833
        
        //i think divide all the above by 2 =?
        //100-4.5 || 500-22.9 || 1000-45.8 || 5000-229 || 10000-458 || 20000-917
        
        //if(i == 9||i ==46||i==92||i==458||i==917||i==1833)
        if(i == 5||i ==23||i==46||i==229||i==458||i==917)
        {
            g.setColour(Colours::whitesmoke.withAlpha(0.7f));
            g.drawLine(0,getHeight() - (getHeight()*skewedProportionX+1.0f), getWidth(),getHeight()- (getHeight()*skewedProportionX+1.0f));
            if(i ==5)
            {
                g.drawText("100 Hz", 0, getHeight() - getHeight()*skewedProportionX+1.0f , getWidth()/10, 10, Justification::centredLeft);
            }
            if(i ==23)
            {
                g.drawText("500 Hz", 0, getHeight() - getHeight()*skewedProportionX+1.0f, getWidth()/10,10, Justification::centredLeft);
            }if(i ==46)
            {
                g.drawText("1KHz", 0, getHeight() - getHeight()*skewedProportionX+1.0f, getWidth()/10,10, Justification::centredLeft);
            }if(i ==229)
            {
                g.drawText("5KHz", 0, getHeight() - getHeight()*skewedProportionX+1.0f, getWidth()/10, 10, Justification::centredLeft);
            }if(i ==458)
            {
                g.drawText("10KHz", 0, getHeight() - getHeight()*skewedProportionX+1.0f, getWidth()/10, 10, Justification::centredLeft);
            }if(i ==917)
            {
                g.drawText("20KHz", 0, getHeight() - getHeight()*skewedProportionX+1.0f, getWidth()/10, 10, Justification::centredLeft);
            }
        }
        
    }
    
    //===============================DRAWING THE ANALAYSER STEREO==============================
    //starting lines
    Path L;
    Path R;
    
    L.startNewSubPath(getWidth()/2, getHeight());
    R.startNewSubPath(getWidth()/2, getHeight());
    
    for ( int i = 0 ; i < fftSize/2 ;i++)
    {
        //get values from fft
        float rRatio = getAndScaleValueR(i);
        float lRatio = getAndScaleValueL(i);
        
        //check difference
        float ratio = rRatio-lRatio;
        
        //(make it more visible)
        ratio = ratio*2;
        
        //trying to smooth out low end
        if(i == 0)
        {
            ratio = 0;
        }
        
        skewedProportionX =   std::exp (std::log ((float) i / (float) (fftSize/2)) * 0.2f);
        
//        //=========================================DRAWING BARS=========================================================
//        //right hand side
//        g.setColour(Colours::grey);
//        if(ratio < 0)
//        {
//            //g.drawRect(getWidth()/2.0f+ (ratio  *getWidth()/2),getHeight()- getHeight()*skewedProportionX ,- (ratio  *getWidth()/2) , 1.0f);
//        }
//        //left hand side
//        g.setColour(Colours::blue);
//        if(ratio> 0)
//        {
//            //g.drawRect(getWidth()/2.0f, getHeight()- getHeight()*skewedProportionX,  (ratio  *getWidth()/2), 1.0f);
//
//        }
        
        
        ratio = ratio * 3;
        
        //=========================================DRAWING DOTS============================================================
        //OUTRIGHT VALUES
        if(processor.panDraw)
        {
           g.setColour(Colours::turquoise.withAlpha(0.4f));
           g.fillEllipse(getWidth()/2 - (ratio  *getWidth()/2)-1.5f, getHeight()- getHeight()*skewedProportionX -1.5f, 3.0f, 3.0f);
            
            g.setColour(Colours::lightblue.withAlpha(1.0f));
            g.fillEllipse(getWidth()/2 - (ratio  *getWidth()/2)-0.5f, getHeight()- getHeight()*skewedProportionX-0.5f , 1.0f, 1.0f);
        }
        else
        {
            //get RMS so always +ve
            float ratio2 = std::pow(ratio, 2);
            ratio2 = std::sqrt(ratio2);
            
            
            g.setColour(Colours::green.withAlpha(0.4f));
            //draw two dots equal width (ratio) from the centre
            //L
            g.fillEllipse(getWidth()/2 - (ratio2  *getWidth()/2)-1.5f, getHeight()- getHeight()*skewedProportionX -1.5f, 3.0f, 3.0f);
            //R
            g.fillEllipse(getWidth()/2 + (ratio2  *getWidth()/2)-1.5f, getHeight()- getHeight()*skewedProportionX -1.5f, 3.0f, 3.0f);
            
            g.setColour(Colours::lightgreen.withAlpha(0.5f));
            //l
            g.fillEllipse(getWidth()/2 - (ratio2  *getWidth()/2)-0.5f, getHeight()- getHeight()*skewedProportionX -0.5f, 1.0f, 1.0f);
            //r
            g.fillEllipse(getWidth()/2 + (ratio2  *getWidth()/2)-0.5f, getHeight()- getHeight()*skewedProportionX-0.5f , 1.0f, 1.0f);
            
        }
//        g.setColour(Colours::white);
//        g.drawText((String)getAndScaleRMS(), 0, 0, getWidth(), getHeight(), Justification::centred);
        
        ratio = 40* ratio * getAndScaleRMS();
        
        //WITH RESPECT TO RMS
        if(processor.panDraw)
         {
            g.setColour(Colours::rebeccapurple.withAlpha(0.7f));
            g.fillEllipse(getWidth()/2 - (ratio  *getWidth()/2)-1.5f, getHeight()- getHeight()*skewedProportionX-1.5f , 3.0f, 3.0f);
         }
         else
         {
             //get RMS so always +ve
             float ratio3 = std::pow(ratio, 2);
             ratio3 = std::sqrt(ratio3);
             
             g.setColour(Colours::rebeccapurple.withAlpha(0.7f));
             //draw two dots equal width (ratio) from the centre
             //L
             g.fillEllipse(getWidth()/2 - (ratio3  *getWidth()/2)-1.5f, getHeight()- getHeight()*skewedProportionX-1.5f , 3.0f, 3.0f);
             //R
             g.fillEllipse(getWidth()/2 + (ratio3  *getWidth()/2)-1.5f, getHeight()- getHeight()*skewedProportionX -1.5f, 3.0f, 3.0f);
             
         }
    
        
        //========================================DRAWLING LINES============================================================
        //so if ratio > 0
        if(ratio>0)
        {
            //add to one path
            L.lineTo(getWidth()/2 - (ratio  *getWidth()/2), getHeight()- getHeight()*skewedProportionX);
        }
        //if ratio < 0
        if(ratio < 0)
        {
            //add to the other path
            R.lineTo(getWidth()/2 - (ratio  *getWidth()/2), getHeight()- getHeight()*skewedProportionX);
        }
        
    }
    
    //end and draw the path
    L.lineTo(getWidth()/2, 0);
    R.lineTo(getWidth()/2, 0);
    L.closeSubPath();
    R.closeSubPath();
    g.setColour(Colours::lightcoral);
//    g.strokePath(L, PathStrokeType(1));
//    g.strokePath(R,PathStrokeType(1));
//
    //draw central line
    g.setColour(Colours::antiquewhite);
    g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight());
    
    
    
}

void Meter1::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

float Meter1::getAndScaleValueL(int i)
{
    //for each bin
    //draw a line
    //at the height of the FFT value
    float val = processor.getFFTAvgL(i);
    
    //value ~ 0-1 (values were coming out at about 800)
    float gain = val/1200;
    //log scale
    float valInDb = Decibels::gainToDecibels(gain);
    //limit between -30 and 6
    float valDB = jlimit(-96.0f, 12.0f, valInDb);
    //scale db back to ratio
    float  hRatio = 1.0f+((valDB-12.0f)/108.0f);
    return hRatio;
}

float Meter1::getAndScaleValueR(int i)
{
    //for each bin
    //draw a line
    //at the height of the FFT value
    float val = processor.getFFTAvgR(i);
    
    //value ~ 0-1 (values were coming out at about 800)
    float gain = val/1200;
    //log scale
    float valInDb = Decibels::gainToDecibels(gain);
    //limit between -30 and 6
    float valDB = jlimit(-96.0f, 12.0f, valInDb);
    //scale db back to ratio
    float  hRatio = 1.0f+((valDB-12.0f)/108.0f);
    return hRatio;
}


float Meter1::getAndScaleRMS()
{
    
    float val = processor.getRMS();
    
    return val;
    
}
