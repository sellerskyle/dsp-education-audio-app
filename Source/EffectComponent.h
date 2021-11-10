/*
  ==============================================================================

    EffectComponent.h
    Created: 4 Oct 2021 4:56:51pm
    Author:  Kyle

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class EffectComponent  : public juce::Component
{
public:
    EffectComponent();
    ~EffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getEffectType();
    int getBitDepth();
    int getSampleRateReductionFactor();
    void setSampleRate(int sampleRate);
    float getDistortionGain();
    int getDistortionType();
    float getDelayTime();
    int getDelayType();
    int getNumRepeats();
    float getDelayLevel();

private:
    int sampleRate = 0;
    juce::StringArray effectTypes = { "Distortion", "Bitcrusher", "Delay"};
    juce::StringArray clippingTypes = { "Hard", "Tanh" };
    juce::ComboBox effectSelector{ "Effect Selector" };
    juce::Label effectTitleLabel{ "EffectTitleLabel", "Effect" };

    //Bitcrusher Controls
    juce::Slider bitDepthSlider{ juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label bitDepthLabel{ "BitDepthLabel", "Bit Depth" };
    juce::Slider sampleRateSlider{ juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label sampleRateLabel{ "SampleRateLabel", "Sample Rate Reduction Factor" };


    //Distortion Controls
    juce::ComboBox distortionTypeSelector{ "Distortion Type Selector" };
    juce::Label distortionSelectorLabel { "distortionSelectorLabel", "Distortion Type" };
    juce::Slider distortionGainSlider{ juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label distortionGainLabel{ "distortionGainLabel", "Gain" };

    //Delay Controls
    juce::ComboBox delayTypeSelector{ "Delay Type Selector" };
    juce::Label delayTypeLabel{ "delayTypeLabel", "Delay Type" };
    juce::StringArray delayTypes = { "FIR", "IIR" };
    juce::Slider delayTimeSlider{ juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label delayTimeLabel{ "delayTimeLabel", "Delay Time" };
    juce::Slider numRepeatsSlider{ juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label numRepeatsLabel{ "numRepeatsLabel", "# Repeats" };
    juce::Slider delayLevelSlider{ juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label delayLevelLabel{ "delayLevelLabel", "Delay Volume" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectComponent)
};
