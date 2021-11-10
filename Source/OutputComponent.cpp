/*
  ==============================================================================

    OutputComponent.cpp
    Created: 4 Oct 2021 4:37:45pm
    Author:  Kyle

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OutputComponent.h"

//==============================================================================
OutputComponent::OutputComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    outputTitleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(outputTitleLabel);

    outputSliderLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(outputSliderLabel);


    outputVolumeSlider.setRange(0.0, 1.0, 0.01);
    outputVolumeSlider.setValue(0.25);
    addAndMakeVisible(outputVolumeSlider);

    outputSliderLabel.attachToComponent(&outputVolumeSlider, false);
}

OutputComponent::~OutputComponent()
{
}

void OutputComponent::paint (juce::Graphics& g)
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
    outputTitleLabel.setBounds(0, 0, int(0.8 * width), int(0.1 * height));
    outputVolumeSlider.setBounds(int(0.1 * width), int(0.2 * height), int(0.8 * width), int(0.75 * height));

}

void OutputComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    repaint();

}

float OutputComponent::getOutputVolume()
{
    return outputVolumeSlider.getValue();
}