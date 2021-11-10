/*
  ==============================================================================

    VisualizationComponent.h
    Created: 26 Oct 2021 10:54:15am
    Author:  Kyle

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class VisualizationComponent  : public juce::Component, public juce::Timer
{
public:
    VisualizationComponent();
    ~VisualizationComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setSignalFrequency(float freq);
    void pushBuffer(const juce::AudioBuffer<float>& buffer);
    enum
    {
        fftOrder = 10,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

    void timerCallback() override
    {
        if (nextFFTBlockReady)
        {
            if (visualSelector.getSelectedIdAsValue() == 2) // Spectrogram
                drawNextLineOfSpectrogram();
            else if (visualSelector.getSelectedIdAsValue() == 3)
                drawNextFrameOfSpectrum();


            nextFFTBlockReady = false;
            repaint();
        }
    }

    void pushNextSampleIntoFifo(float sample) noexcept
    {
        // if the fifo contains enough data, set a flag to say
        // that the next line should now be rendered..
        if (fifoIndex == fftSize)
        {
            if (!nextFFTBlockReady)
            {
                juce::zeromem(fftData, sizeof(fftData));
                memcpy(fftData, fifo, sizeof(fifo));
                nextFFTBlockReady = true;
            }

            fifoIndex = 0;
        }

        fifo[fifoIndex++] = sample;
    }

    void drawNextLineOfSpectrogram()
    {
        auto rightHandEdge = spectrogramImage.getWidth() - 1;
        auto imageHeight = spectrogramImage.getHeight();

        // first, shuffle our image leftwards by 1 pixel..
        spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);

        // then render our FFT data..
        forwardFFT.performFrequencyOnlyForwardTransform(fftData);

        // find the range of values produced, so we can scale our rendering to
        // show up the detail clearly
        auto maxLevel = juce::FloatVectorOperations::findMinAndMax(fftData, fftSize / 2);

        for (auto y = 1; y < imageHeight; ++y)
        {
            auto skewedProportionY = 1.0f - std::exp(std::log((float)y / (float)imageHeight) * 0.2f);
            auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionY * (int)fftSize / 2));
            auto level = juce::jmap(fftData[fftDataIndex], 0.0f, juce::jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

            spectrogramImage.setPixelAt(rightHandEdge, y, juce::Colour::fromHSV(level, 1.0f, level, 1.0f));
        }
    }

    void drawNextFrameOfSpectrum()
    {
        // first apply a windowing function to our data
        window.multiplyWithWindowingTable(fftData, fftSize);

        // then render our FFT data..
        forwardFFT.performFrequencyOnlyForwardTransform(fftData);

        auto mindB = -100.0f;
        auto maxdB = 0.0f;

        for (int i = 0; i < scopeSize; ++i)
        {
            auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
            auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
            auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftData[fftDataIndex])
                - juce::Decibels::gainToDecibels((float)fftSize)),
                mindB, maxdB, 0.0f, 1.0f);

            scopeData[i] = level;
        }
    }

private:

    juce::AudioVisualiserComponent audioVisComp{ 1 }; // 1 = num channels

    juce::StringArray visualTypes = { "Oscilloscope", "Spectrogram", "Spectrum Analyzer"};
    juce::ComboBox visualSelector{ "Input Selector" };

    //Spectrogram & Spectrum Analyzer
    juce::dsp::FFT forwardFFT;
    juce::dsp::WindowingFunction<float> window;
    juce::Image spectrogramImage;

    float fifo[fftSize];
    float fftData[2 * fftSize];
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;
    float scopeData[scopeSize];


    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualizationComponent)
};
