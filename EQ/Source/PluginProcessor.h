/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "BiquadFilter.h"
#include "toneGenerator.h"


#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class EqAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    EqAudioProcessor();

    
    ~EqAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    //
    void pushNextSampleIntoFifo(float sample) noexcept;
    float passParemeter();
    //
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
    
    
    //==============================================================================
    //float noteOnVel;
    
    double freq, gain, q;
    int control;
    
    enum
    {
        fftOrder = 10,
        fftSize = 1<<fftOrder
    };

private:
    double SR;
    
    
    BiquadFilter filterL;
    BiquadFilter filterR;
    ToneGenerator toneL;
    ToneGenerator toneR;
    
    //dsp::FFT performFrequencyOnlyForwardTransform;
    
    dsp::FFT forwardFFT;
    float fifo[fftSize];
    float fftData[2*fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
     

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqAudioProcessor)
};
