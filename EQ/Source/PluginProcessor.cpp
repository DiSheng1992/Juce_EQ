/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EqAudioProcessor::EqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     :  forwardFFT(fftOrder),
AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )

#endif
{
}

EqAudioProcessor::~EqAudioProcessor()
{
}

//==============================================================================
const String EqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EqAudioProcessor::setCurrentProgram (int index)
{
}

const String EqAudioProcessor::getProgramName (int index)
{
    return {};
}

void EqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    
    SR = sampleRate;
    
    toneL.initialiseWave(500, 0.25, SR, ToneGenerator::NOISE);
    toneR.initialiseWave(500, 0.25, SR, ToneGenerator::NOISE);
    
    freq = 10000;
    gain = 5;
    q = 10;
    control = 0;
    

}

void EqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EqAudioProcessor::pushNextSampleIntoFifo (float sample) noexcept
{
    // if the fifo contains enough data, set a flag to say
    // that the next line should now be rendered..
    
    if (fifoIndex == fftSize)    // [8]
    {
        if (! nextFFTBlockReady) // [9]
        {
            zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }
        fifoIndex = 0;
    }
    fifo[fifoIndex++] = sample;  // [9]
}

float EqAudioProcessor::passParemeter() {
    forwardFFT.performFrequencyOnlyForwardTransform (fftData);
    float a;
    a = *std::max_element(fftData,fftData+fftSize)*22050/fftSize;
    return a;
}
void EqAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        
        float output;
        float input;
        
        BiquadFilter::filterType type;
        
        //save audio frame:
        auto* inputData = buffer.getReadPointer (channel);
        //float audio_frame[buffer.getNumSamples()];
        
        for (int i = 0; i < buffer.getNumSamples(); i++){
            pushNextSampleIntoFifo (inputData[i]);
            //audio_frame[i] = inputData[i];
        }
        
        //forwardFFT.performFrequencyOnlyForwardTransform(audio_frame);
        float frequency;
        int addGain = 0;
        frequency = passParemeter();
        if (frequency > 1000){
            addGain = 1;
        }

        
        
        
        
        
        
        //presetings:
        static float pre_set_gain[10][10] = {
            {3,    3, 0, 0,  0,  0, 0, 0,  3,  3},       /* Normal Preset */
            {5,    5, 3, 3, 2, 2, 4, 4,  4,  4},       /* Classical Preset */
            {6,    6, 0, 0,  2,  2, 4, 4,  1,  1},       /* Dance Preset */
            {0,    0, 0, 0,  0,  0, 0, 0,  0,  0},       /* Flat Preset */
            {3,    3, 0, 0,  0,  0, 2, 2, 1, 1},       /* Folk Preset */
            {4,    4, 1, 1,  9,  9, 3, 3,  0,  0},       /* Heavy Metal Preset */
            {5,    5, 3, 3,  0,  0, 1, 1,  3,  3},       /* Hip Hop Preset */
            {4,    4, 2, 2, 2, 2, 2, 2,  5,  5},       /* Jazz Preset */
            {1,  1, 2, 2,  5,  5, 1, 1, 2, 2},       /* Pop Preset */
            {5,    5, 3, 3, 1, 1, 3, 3,  5,  5} };     /* Rock Preset */
        
        

        static float pre_set_q[10] = {1.03, 0.555, 0.255, 0.25, 0.253, 0.244, 0.258, 0.246, 0.317, 1.502};
        static float pre_set_frequency[10] = {31, 62, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};

        
        //processing:
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            
            type = static_cast<BiquadFilter::filterType>(0);
            
            if (channel == 0){
                input = toneL.getValue();
                filterL.setParameters(pre_set_frequency[0], pre_set_gain[control][0]+addGain, pre_set_q[0], type);
                filterL.addSample(input);
                output = filterL.getSample();

                
            }
            else{
                input = toneR.getValue();
                filterR.setParameters(pre_set_frequency[0], pre_set_gain[control][0]+addGain, pre_set_q[0], type);
                filterR.addSample(input);
                output = filterR.getSample();
            }
            
            for (int j = 1; j<=8; j++){
                type = static_cast<BiquadFilter::filterType>(2);
                if (channel == 0){
                    input = filterL.getSample();
                    filterL.setParameters(pre_set_frequency[j], pre_set_gain[control][j]+addGain, pre_set_q[j], type);
                    filterL.addSample(input);
                    output = filterL.getSample();
                    
                }
                else{
                    input = filterR.getSample();
                    filterR.setParameters(pre_set_frequency[j], pre_set_gain[control][j], pre_set_q[j], type);
                    filterR.addSample(input);
                    output = filterR.getSample();
                }
            }


            type = static_cast<BiquadFilter::filterType>(1);

            if (channel == 0){
            input = filterL.getSample();
            filterL.setParameters(pre_set_frequency[9], pre_set_gain[control][9], pre_set_q[9], type);
            filterL.addSample(input);
            output = filterL.getSample();
            
            }
            else{
            input = filterR.getSample();
            filterR.setParameters(pre_set_frequency[9], pre_set_gain[control][9], pre_set_q[9], type);
            filterR.addSample(input);
            output = filterR.getSample();
            }
                
             
            
            channelData[i] = output;
        }
 
    }
  
  
}

//==============================================================================
bool EqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EqAudioProcessor::createEditor()
{
    return new EqAudioProcessorEditor (*this);
}

//==============================================================================
void EqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EqAudioProcessor()
    ;
}
