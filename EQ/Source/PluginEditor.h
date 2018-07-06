/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class EqAudioProcessorEditor  : public AudioProcessorEditor,
                                private Slider::Listener,
                                private ComboBox::Listener
{
public:
    EqAudioProcessorEditor (EqAudioProcessor&);
    ~EqAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    void sliderValueChanged (Slider* slider) override;
    void comboBoxChanged (ComboBox* comboBox) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EqAudioProcessor& processor;

    //Slider midiVolume;
    
    
    Slider frequencySlider;
    Label frequencyLabel;
    
    Slider gainSlider;
    Label gainLabel;
    
    Slider qSlider;
    Label qLabel;
    
    ComboBox filterMenu;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqAudioProcessorEditor)
};
