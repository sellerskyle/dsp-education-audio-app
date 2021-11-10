/*
  ==============================================================================

    VisualizationComponent.cpp
    Created: 26 Oct 2021 10:54:15am
    Author:  Kyle

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VisualizationComponent.h"

//==============================================================================
VisualizationComponent::VisualizationComponent() : forwardFFT(fftOrder),
                                                spectrogramImage(juce::Image::RGB, 512, 512, true),
                                                window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    startTimerHz(60);

    visualSelector.addItemList(visualTypes, 1);
    visualSelector.setSelectedId(1);
    addAndMakeVisible(visualSelector);

    audioVisComp.setColours(juce::Colours::transparentBlack, juce::Colours::white);
    audioVisComp.setBufferSize(200); //Changes the number of samples that the visualiser keeps in its history.
                                //Note that this value refers to the number of averaged sample blocks, and each block is calculated as the peak of a number of incoming audio samples.To set the number of incoming samples per block, use setSamplesPerBlock().
    audioVisComp.setSamplesPerBlock(4); //number of samples averaged per incoming "block"
    audioVisComp.setRepaintRate(60);
    audioVisComp.setOpaque(false);
    audioVisComp.addMouseListener(this, true);
    addAndMakeVisible(audioVisComp);
}

VisualizationComponent::~VisualizationComponent()
{
}

void VisualizationComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    auto width = getLocalBounds().getWidth();
    auto height = getLocalBounds().getHeight();
    auto visualArea = juce::Rectangle<float>{ 0.0f, 0.1f * height, 1.0f * width, 0.9f * height };

    if (visualSelector.getSelectedIdAsValue() == 2) // Spectrogram
        g.fillAll(juce::Colours::black);
    else
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    visualSelector.setBounds(0.1* width, 0.05*height, 0.5*width, 0.05*height);

    audioVisComp.setVisible(false);
    if (visualSelector.getSelectedIdAsValue() == 1) // Oscope
    {
        audioVisComp.setVisible(true);
        audioVisComp.setBounds(0, 0.1 * height, width, height);
    }
    else if (visualSelector.getSelectedIdAsValue() == 2) //Spectrogram
    {
        g.drawImage(spectrogramImage, visualArea);
    }
    else if (visualSelector.getSelectedIdAsValue() == 3) //Spectrum Analyzer
    {
        for (int i = 1; i < scopeSize; ++i)
        {
            

            g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, int(visualArea.getWidth())),
                                  juce::jmap(scopeData[i - 1], 0.0f, 1.0f, visualArea.getHeight(), visualArea.getY()),
                          (float)juce::jmap(i,     0, scopeSize - 1, 0, int(visualArea.getWidth())),
                                  juce::jmap(scopeData[i],     0.0f, 1.0f, visualArea.getHeight(), visualArea.getY()) });
        }
    }

}

void VisualizationComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    repaint();

}

void VisualizationComponent::setSignalFrequency(float freq)
{
    audioVisComp.setBufferSize(freq);
}

void VisualizationComponent::pushBuffer(const juce::AudioBuffer<float>& buffer)
{
    audioVisComp.pushBuffer(buffer);
}

