/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WidenessAnalyserAudioProcessor::WidenessAnalyserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: forwardFFT(fftOrder), fifoIndexL(0),fifoIndexR(0), currentSampleRate(0.0f),

AudioProcessor(BusesProperties().withInput("Main", AudioChannelSet::stereo())
               .withOutput("Main", AudioChannelSet::stereo())                       )
#endif
{
}

WidenessAnalyserAudioProcessor::~WidenessAnalyserAudioProcessor()
{
}

//==============================================================================
const String WidenessAnalyserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WidenessAnalyserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WidenessAnalyserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WidenessAnalyserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WidenessAnalyserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WidenessAnalyserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WidenessAnalyserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WidenessAnalyserAudioProcessor::setCurrentProgram (int index)
{
}

const String WidenessAnalyserAudioProcessor::getProgramName (int index)
{
    return {};
}

void WidenessAnalyserAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WidenessAnalyserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WidenessAnalyserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WidenessAnalyserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WidenessAnalyserAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    //define two audio buffers
    AudioSampleBuffer mainInputOutput = getBusBuffer(buffer, true, 0);
    
    
    ScopedNoDenormals noDenormals;
    
    auto mainInputChannels = mainInputOutput.getNumChannels();
    auto mainOutputChannels = mainInputOutput.getNumChannels();
    
    
    //clear main
    for (auto i = mainInputChannels; i < mainOutputChannels; ++i)
        mainInputOutput.clear(i, 0, mainInputOutput.getNumSamples());
    
    auto* channelDataL = mainInputOutput.getWritePointer(0);
    
    
    //rms
    float sumL=0;
    
    for (auto sample = 0; sample < mainInputOutput.getNumSamples(); ++sample)
    {
        float const samp = channelDataL[sample];
        
        sumL = sumL+ samp;
        
        pushNextSampleIntoFifoL(samp);
    }
    
    auto* channelDataR = mainInputOutput.getWritePointer(1);
    
    
    //rms
    float sumR = 0;
    
    for (auto sample = 0; sample < mainInputOutput.getNumSamples(); ++sample)
    {
        float const samp = channelDataR[sample];
        
        sumR = sumR+samp;
        
        pushNextSampleIntoFifoR(samp);
    }
    
    RMSL = sumL/mainInputOutput.getNumSamples();
    
    RMSR = sumR/mainInputOutput.getNumSamples();;
    
    //STORE DATA FOR AVERAGES - L
    //for the length
    for(int i =  0; i < fftSize/2;i++)
    {
        //add the fft values at one of the 10 points
        
        fftLogL[i][c] = getFFTL(i);
    }
   
    
    //STORE DATA FOR AVERAGES - L
    //for the length
    for(int i =  0; i < fftSize/2;i++)
    {
        //add the fft values at one of the 10 points
        
        fftLogR[i][c] = getFFTR(i);
    }
    //incrase count (so we go to next row of 2d array nec time)
    c++;
    //if we are at max of array then go back to start
    if(c == thisNoOfAvg)
    {
        c = 0;
    }
    
    //SEE IF THE AVG SHOULD CHANGE
    if(nextAvg != thisNoOfAvg && nextAvg>5 && nextAvg<1000)
    {
        thisNoOfAvg = nextAvg;
        c=0;
    }
}

//==============================================================================
bool WidenessAnalyserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WidenessAnalyserAudioProcessor::createEditor()
{
    return new WidenessAnalyserAudioProcessorEditor (*this);
}

//==============================================================================
void WidenessAnalyserAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WidenessAnalyserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
void WidenessAnalyserAudioProcessor::pushNextSampleIntoFifoL(float sample) noexcept
{
    //if the index has reached the end
    if (fifoIndexL == fftSize)
    {
        //if next block wasn't already ready
        if (!nextFFtBlockReadyL)
        {
            zeromem(fftDataL, sizeof(fftDataL));//clears fftData
            memcpy(fftDataL, fifoL, sizeof(fifoL));//copies memory from fifo to fftData
            nextFFtBlockReadyL = true;//ready to perform FFT
        }
        fifoIndexL = 0;//resests the array counter on fifo to 0
    }
    //if next block is ready
    if (nextFFtBlockReadyL)
    {
        //perform fft calculations
        forwardFFT.performFrequencyOnlyForwardTransform(fftDataL);
        nextFFtBlockReadyL = false;
        readyToDrawL = true;
    }
    
    fifoL[fifoIndexL++] = sample;//information from sample gets pushed into fifo at array point fifoIndex.
    
}


void WidenessAnalyserAudioProcessor::pushNextSampleIntoFifoR(float sample) noexcept
{
    //Ok, so here we are filling fifo with sample data. When fifo gets full, that data
    //gets copied to the first half of fftData. fifo then gets rewritten by next set of data.
    //Also to make the loop work fftData needs to be cleared
    
    
    //if the index has reached the end
    if (fifoIndexR == fftSize)
    {
        //if next block wasn't already ready
        if (!nextFFtBlockReadyR)
        {
            zeromem(fftDataR, sizeof(fftDataR));//clears fftData
            memcpy(fftDataR, fifoR, sizeof(fifoR));//copies memory from fifo to fftData
            nextFFtBlockReadyR = true;//ready to perform FFT
        }
        fifoIndexR = 0;//resests the array counter on fifo to 0
    }
    //if next block is ready
    if (nextFFtBlockReadyR)
    {
        //perform fft calculations
        forwardFFT.performFrequencyOnlyForwardTransform(fftDataR);
        nextFFtBlockReadyR = false;
        readyToDrawR = true;
    }
    
    fifoR[fifoIndexR++] = sample;//information from sample gets pushed into fifo at array point fifoIndex.
    
}




int WidenessAnalyserAudioProcessor::getFFTSizeL()
{
    return fftSize;
}


int WidenessAnalyserAudioProcessor::getFFTSizeR()
{
    return fftSize;
}



float WidenessAnalyserAudioProcessor::getFFTL(int index)
{
    return fftDataL[index];
}


float WidenessAnalyserAudioProcessor::getFFTR(int index)
{
    return fftDataR[index];
}

float WidenessAnalyserAudioProcessor::getFFTAvgL(int index)
{
    float total = 0;
    for (int i = 0 ; i <thisNoOfAvg ; i++)
    {
        float thisVal = fftLogL[index][i];
        
        total = total+thisVal;
    }
    float fftAVGL = total/thisNoOfAvg;
    
    return fftAVGL;
    
}

float WidenessAnalyserAudioProcessor::getFFTAvgR(int index)
{
    float total = 0;
    for (int i = 0 ; i <thisNoOfAvg ; i++)
    {
        float thisVal = fftLogR[index][i];
        
        total = total+thisVal;
    }
    float fftAVGR = total/thisNoOfAvg;
    
    return fftAVGR;
    
}

float WidenessAnalyserAudioProcessor::getMaxLevelL()
{
    juce::Range<float> maxAndMin = FloatVectorOperations::findMinAndMax(fftDataL, fftSize);
    
    return maxAndMin.getEnd();
}


float WidenessAnalyserAudioProcessor::getMaxLevelR()
{
    juce::Range<float> maxAndMin = FloatVectorOperations::findMinAndMax(fftDataR, fftSize);
    
    return maxAndMin.getEnd();
}

float WidenessAnalyserAudioProcessor::getRMS()
{
    return (RMSL + RMSR)/2;
    
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WidenessAnalyserAudioProcessor();
}
