/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class WidenessAnalyserAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    WidenessAnalyserAudioProcessor();
    ~WidenessAnalyserAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void pushNextSampleIntoFifoL(float sample) noexcept;
    void pushNextSampleIntoFifoR(float sample) noexcept;
    
    
    //order (complexity of fft)
    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder
    };
    
    
    //functions to call values from the editor
    float getFFTL(int index);
    float getMaxLevelL();
    int getFFTSizeL();
    
    
    float getFFTR(int index);
    float getMaxLevelR();
    int getFFTSizeR();
    
    //fftdata arrays
    float fftDataL[2 * fftSize];
    float fftDataR[2 * fftSize];
    
    //define fft
    dsp::FFT forwardFFT;
    
    //booleans for logic
    bool nextFFtBlockReadyL = false;
    bool nextFFtBlockReadyR = false;
    bool readyToDrawL = false;
    bool readyToDrawR = false;
    
    float frequency;
    
    
    
    //for taking averages
    
    //total size (number of averages of 2nd dimension of array
    enum{ totalNoOfAvg = 500};
    float thisNoOfAvg = 10;
    int nextAvg;
    
    float fftLogL[fftSize/2][totalNoOfAvg];
    float getFFTAvgL(int index);
    
    float fftLogR[fftSize/2][totalNoOfAvg];
    float getFFTAvgR(int index);
    
    //draw types
    bool panDraw = false;
    
    
    //get RMS
    
    float getRMS();
    float RMSL=0;
    float RMSR=0;
    
private:
    //variables
    double currentSampleRate;
    
    //count for averaging
    int c = 0;
    
    
    //fifo array for DSP
    float fifoL[fftSize];
    int fifoIndexL;
    
    
    float fifoR[fftSize];
    int fifoIndexR;
    
    
    int numberOfBins = fftSize / 2;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WidenessAnalyserAudioProcessor)
};
