/*
  ==============================================================================

    InputComponent.cpp
    Created: 30 Sep 2021 3:23:37pm
    Author:  Kyle

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputComponent.h"

//==============================================================================
InputComponent::InputComponent() : state(Stopped)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    inputTitleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(inputTitleLabel);

    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volumeLabel);

    frequencyLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(frequencyLabel);



    inputSelector.addItemList(inputTypes, 1);
    addAndMakeVisible(inputSelector);

    inputVolumeSlider.setRange(0.0, 1.0, 0.01);
    inputVolumeSlider.setValue(0.25);
    addAndMakeVisible(inputVolumeSlider);

    inputFrequencySlider.setRange(100.0, 5000.0, 100.0);
    inputVolumeSlider.setValue(0.25);
    addAndMakeVisible(inputFrequencySlider);

    frequencyLabel.attachToComponent(&inputFrequencySlider, false);
    volumeLabel.attachToComponent(&inputVolumeSlider, false);


    formatManager.registerBasicFormats();       // [1]
    transportSource.addChangeListener(this);   // [2]
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    stopButton.setEnabled(false);
}

InputComponent::~InputComponent()
{
    transportSource.releaseResources();
}



void InputComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    int width = getLocalBounds().getWidth();
    int height = getLocalBounds().getHeight();

    auto inputSelection = getInputType();


    inputTitleLabel.setBounds(0, 0, int(0.8 * width), int(0.1 * height));
    inputSelector.setBounds(int(0.1 * width), int(0.1 * height), int(0.8 * width), int(0.05 * height));

    if (inputSelection == 7) //User file
    {
        openButton.setVisible(true);
        playButton.setVisible(true);
        stopButton.setVisible(true);
        openButton.setBounds(int(0.1 * width), int(0.2 * height), int(0.8 * width), int(0.05 * height));
        playButton.setBounds(int(0.1 * width), int(0.25 * height), int(0.8 * width), int(0.05 * height));
        stopButton.setBounds(int(0.1 * width), int(0.3 * height), int(0.8 * width), int(0.05 * height));
        inputVolumeSlider.setBounds(int(0.1 * width), int(0.40 * height), int(0.8 * width), int(0.30 * height));
        inputFrequencySlider.setBounds(int(0.1 * width), int(0.75 * height), int(0.8 * width), int(0.2 * height));
    }
    else
    {
        openButton.setVisible(false);
        playButton.setVisible(false);
        stopButton.setVisible(false);

        inputVolumeSlider.setBounds(int(0.1 * width), int(0.2 * height), int(0.8 * width), int(0.5 * height));
        inputFrequencySlider.setBounds(int(0.1 * width), int(0.75 * height), int(0.8 * width), int(0.2 * height));
    }

}

void InputComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    // x y width height

    repaint();

    
    
}

float InputComponent::getInputVolume()
{
    return inputVolumeSlider.getValue();
}

int InputComponent::getInputType()
{
    return inputSelector.getSelectedId();
}

float InputComponent::getFrequency()
{
    return inputFrequencySlider.getValue();
}




