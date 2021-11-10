/*
  ==============================================================================

    InputComponent.h
    Created: 30 Sep 2021 3:23:37pm
    Author:  Kyle

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class InputComponent  : public juce::Component,
    public juce::ChangeListener
{
public:
    InputComponent();
    ~InputComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    float getInputVolume();

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    int getInputType();
    float getFrequency();
    TransportState state = Stopped;
    TransportState getState()
    {
        return state;
    }
    juce::AudioTransportSource transportSource;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
        {
            if (transportSource.isPlaying())
                changeState(Playing);
            else
                changeState(Stopped);
        }
    }


private:


    juce::StringArray inputTypes = { "Sine", "Square", "Triangle", "Sawtooth", "Noise", "Input", "Uploaded File" };
    juce::ComboBox inputSelector{ "Input Selector" };
    juce::Slider inputVolumeSlider{ juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Slider inputFrequencySlider{ juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow };
    juce::Label inputTitleLabel{ "InputTitleLabel", "Input" };
    juce::Label volumeLabel{ "VolumeLabel", "Gain" };
    juce::Label frequencyLabel{ "FrequencyLabel", "Frequency / Resolution" };

    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    std::unique_ptr<juce::FileChooser> chooser;
    

    juce::AudioFormatManager formatManager;


    void changeState(TransportState newState)
    {
        if (state != newState)
        {
            state = newState;

            switch (state)
            {
            case Stopped:                           // [3]
                stopButton.setEnabled(false);
                playButton.setEnabled(true);
                transportSource.setPosition(0.0);
                break;

            case Starting:                          // [4]
                playButton.setEnabled(false);
                transportSource.start();
                break;

            case Playing:                           // [5]
                stopButton.setEnabled(true);
                break;

            case Stopping:                          // [6]
                transportSource.stop();
                break;
            }
        }
    }

    void openButtonClicked()
    {
        chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
            juce::File{},
            "*.wav");                     // [7]
        auto chooserFlags = juce::FileBrowserComponent::openMode
            | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)           // [8]
        {
            auto file = fc.getResult();

            if (file != juce::File{})                                                      // [9]
            {
                auto* reader = formatManager.createReaderFor(file);                 // [10]

                if (reader != nullptr)
                {
                    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);   // [11]
                    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);       // [12]
                    playButton.setEnabled(true);                                                      // [13]
                    readerSource.reset(newSource.release());                                          // [14]
                }
            }
        });
    }

    void playButtonClicked()
    {
        changeState(Starting);
    }

    void stopButtonClicked()
    {
        changeState(Stopping);
    }


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputComponent)
};
