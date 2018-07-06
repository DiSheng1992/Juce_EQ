/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EqAudioProcessorEditor::EqAudioProcessorEditor (EqAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    /*
    // these define the parameters of our slider object
    midiVolume.setSliderStyle (Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled (true, false, this);
    midiVolume.setTextValueSuffix (" Volume");
    midiVolume.setValue(1.0);
    // this function adds the slider to the editor
    addAndMakeVisible (&midiVolume);
    
    // add the listener to the slider
    midiVolume.addListener (this);
    */
    
    /*
    addAndMakeVisible(frequencySlider);
    frequencySlider.setSliderStyle(Slider::RotaryVerticalDrag);
    frequencySlider.setRange(20, 20000, 1);
    frequencySlider.setValue(10000);
    frequencySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    frequencySlider.addListener(this);
    //addAndMakeVisible(&frequencySlider);
    
    
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", NotificationType::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, true);
    frequencyLabel.setJustificationType(Justification::centred);
    //addAndMakeVisible(&frequencyLabel);
    */
    
    
    gainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    gainSlider.setRange(0, 11, 1);
    gainSlider.setValue(5);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    gainSlider.addListener(this);
    
    addAndMakeVisible(&gainSlider);
    
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setText("Gain", NotificationType::dontSendNotification);
    gainLabel.setJustificationType(Justification::centred);
    
    /*
    qSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    qSlider.setRange(0.1, 20, 0.1);
    qSlider.setValue(10);
    qSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    qSlider.addListener(this);
    
    addAndMakeVisible(&qSlider);
    
    //addAndMakeVisible(qLabel);
    qLabel.attachToComponent(&qSlider, false);
    qLabel.setText("Q", NotificationType::dontSendNotification);
    qLabel.setJustificationType(Justification::centred);
    */
    
    
    filterMenu.addItem("Lowpass", 1);
    filterMenu.addItem("Peak1", 2);
    filterMenu.addItem("Peak2", 3);
    filterMenu.addItem("Peak3", 4);
    filterMenu.addItem("Peak4", 5);
    filterMenu.addItem("Peak5", 6);
    filterMenu.addItem("Peak6", 7);
    filterMenu.addItem("Peak7", 8);
    filterMenu.addItem("Peak8", 9);
    filterMenu.addItem("Hightpass", 10);
    filterMenu.setSelectedId(0);
    filterMenu.addListener(this);
    
    addAndMakeVisible(&filterMenu);

}


EqAudioProcessorEditor::~EqAudioProcessorEditor()
{
}

//==============================================================================
void EqAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // fill the whole window white
    g.fillAll (Colours::white);
    // set the current drawing colour to black
    g.setColour (Colours::black);
    // set the font size and draw text to the screen
    g.setFont (10.0f);
    //g.drawFittedText ("Midi Volume", 0, 0, getWidth(), 30, Justification::centred, 1);
}


void EqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    //midiVolume.setBounds (40, 30, 20, getHeight() - 60);
    
    frequencySlider.setBounds(40, 40, 100, 100);
    gainSlider.setBounds(40, 170, 100, 100);
    qSlider.setBounds(170, 40, 100, 100);
    filterMenu.setBounds(170, 170, 100, 100);
}


//================================================================================
void EqAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    //processor.noteOnVel = midiVolume.getValue();
    

    processor.freq = frequencySlider.getValue();
    processor.gain = gainSlider.getValue();
    processor.q = qSlider.getValue();
    
    
}

void EqAudioProcessorEditor::comboBoxChanged (ComboBox* comboBox)
{
    processor.control = filterMenu.getSelectedId() - 1;
}
