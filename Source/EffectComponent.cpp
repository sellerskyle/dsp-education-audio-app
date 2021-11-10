/*
  ==============================================================================

    EffectComponent.cpp
    Created: 4 Oct 2021 4:56:51pm
    Author:  Kyle

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EffectComponent.h"

//==============================================================================
EffectComponent::EffectComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    effectTitleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(effectTitleLabel);

    bitDepthLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(bitDepthLabel);

    sampleRateLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(sampleRateLabel);

    distortionSelectorLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(distortionSelectorLabel);

    distortionGainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(distortionGainLabel);

    delayTypeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(delayTypeLabel);

    delayTimeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(delayTimeLabel);

    numRepeatsLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(numRepeatsLabel);

    delayLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(delayLevelLabel);


    effectSelector.addItemList(effectTypes, 1);
    addAndMakeVisible(effectSelector);

    bitDepthSlider.setRange(1, 16, 1);
    bitDepthSlider.setValue(16);
    addAndMakeVisible(bitDepthSlider);

    sampleRateSlider.setRange(1, 10, 1);
    sampleRateSlider.setValue(1);
    addAndMakeVisible(sampleRateSlider);

    distortionTypeSelector.addItemList(clippingTypes, 1);
    distortionTypeSelector.setSelectedId(1);
    addAndMakeVisible(distortionTypeSelector);

    distortionGainSlider.setRange(1, 100, 0.05);
    distortionGainSlider.setSkewFactor(0.2);
    distortionGainSlider.setValue(1);
    addAndMakeVisible(distortionGainSlider);

    delayTypeSelector.addItemList(delayTypes, 1);
    delayTypeSelector.setSelectedId(1);
    addAndMakeVisible(delayTypeSelector);

    delayTimeSlider.setRange(0.0f, 1.0f, 0.001);
    delayTimeSlider.setValue(0.0);
    addAndMakeVisible(delayTimeSlider);

    numRepeatsSlider.setRange(0, 3, 1);
    numRepeatsSlider.setValue(1);
    addAndMakeVisible(numRepeatsSlider);

    delayLevelSlider.setRange(0.0f, 1.0f, 0.01f);
    delayLevelSlider.setValue(0.5f);
    addAndMakeVisible(delayLevelSlider);

    bitDepthLabel.attachToComponent(&bitDepthSlider, false);
    sampleRateLabel.attachToComponent(&sampleRateSlider, false);
    distortionSelectorLabel.attachToComponent(&distortionTypeSelector, false);
    distortionGainLabel.attachToComponent(&distortionGainSlider, false);
    delayTypeLabel.attachToComponent(&delayTypeSelector, false);
    delayTimeLabel.attachToComponent(&delayTimeSlider, false);
    numRepeatsLabel.attachToComponent(&numRepeatsSlider, false);
    delayLevelLabel.attachToComponent(&delayLevelSlider, false);

}



EffectComponent::~EffectComponent()
{
}

void EffectComponent::paint (juce::Graphics& g)
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
    // All styles are (x, y, width, height)
    effectTitleLabel.setBounds(0, 0, int(0.8 * width), int(0.1 * height));
    effectSelector.setBounds(int(0.1 * width), int(0.1 * height), int(0.8 * width), int(0.05 * height));
    distortionTypeSelector.setVisible(false);
    distortionGainSlider.setVisible(false);
    bitDepthSlider.setVisible(false);
    sampleRateSlider.setVisible(false);

    delayTypeSelector.setVisible(false);
    delayTimeSlider.setVisible(false);
    numRepeatsSlider.setVisible(false);
    delayLevelSlider.setVisible(false);

    switch (this->getEffectType())
    {
    case 1:



        distortionTypeSelector.setVisible(true);
        distortionGainSlider.setVisible(true);
        distortionTypeSelector.setBounds(int(0.1 * width), int(0.2 * height), int(0.8 * width), int(0.05 * height));
        distortionGainSlider.setBounds(int(0.1 * width), int(0.3 * height), int(0.8 * width), int(0.65 * height));

        break;
    case 2:


        bitDepthSlider.setVisible(true);
        sampleRateSlider.setVisible(true);
        bitDepthSlider.setBounds(int(0.1 * width), int(0.2 * height), int(0.8 * width), int(0.35 * height));
        sampleRateSlider.setBounds(int(0.1 * width), int(0.60 * height), int(0.8 * width), int(0.35 * height));
        break;

    case 3:
        delayTypeSelector.setVisible(true);
        delayTimeSlider.setVisible(true);
        delayLevelSlider.setVisible(true);
        delayTypeSelector.setBounds(int(0.1 * width), int(0.2 * height), int(0.8 * width), int(0.05 * height));
        if (getDelayType() == 1)
        {
            numRepeatsSlider.setVisible(true);
            delayTimeSlider.setBounds(int(0.1 * width), int(0.30 * height), int(0.8 * width), int(0.20 * height));
            delayLevelSlider.setBounds(int(0.1 * width), int(0.55 * height), int(0.8 * width), int(0.20 * height));
            numRepeatsSlider.setBounds(int(0.1 * width), int(0.8 * height), int(0.8 * width), int(0.15 * height));
        }
        else 
        {
            delayTimeSlider.setBounds(int(0.1 * width), int(0.30 * height), int(0.8 * width), int(0.30 * height));
            delayLevelSlider.setBounds(int(0.1 * width), int(0.65 * height), int(0.8 * width), int(0.30 * height));
        }
        
    default:
        break;
    }
}

void EffectComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    repaint();


}

int EffectComponent::getEffectType()
{
    return effectSelector.getSelectedId();
}

int EffectComponent::getBitDepth()
{
    return bitDepthSlider.getValue();
}

int EffectComponent::getSampleRateReductionFactor()
{
    return sampleRateSlider.getValue();
}

void EffectComponent::setSampleRate(int sampleRate)
{
    this->sampleRate = sampleRate;
}

float EffectComponent::getDistortionGain()
{
    return distortionGainSlider.getValue();
}

int EffectComponent::getDistortionType()
{
    return distortionTypeSelector.getSelectedId();
}

float EffectComponent::getDelayTime()
{
    return delayTimeSlider.getValue();
}

int EffectComponent::getDelayType()
{
    return delayTypeSelector.getSelectedId();
}

int EffectComponent::getNumRepeats()
{
    return numRepeatsSlider.getValue();
}

float EffectComponent::getDelayLevel()
{
    return delayLevelSlider.getValue();
}

