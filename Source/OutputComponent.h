/*
  ==============================================================================

    OutputComponent.h
    Created: 4 Oct 2021 4:37:45pm
    Author:  Kyle

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OutputComponent  : public juce::Component
{
public:
    OutputComponent();
    ~OutputComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    float getOutputVolume();

private:
    juce::Slider outputVolumeSlider{ juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label outputTitleLabel{ "OutputTitleLabel", "Output" };
    juce::Label outputSliderLabel{ "OutputSliderLabel", "Volume" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputComponent)
};
