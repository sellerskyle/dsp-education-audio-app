#pragma once

#include <JuceHeader.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "InputComponent.h"
#include "OutputComponent.h"
#include "EffectComponent.h"
#include "VisualizationComponent.h"

#define ANTICIPATED_SAMPLE_RATE 44100


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;


private:
    //==============================================================================
    // Your private member variables go here...

    enum inputType {
        Sine = 1,
        Square,
        Triangle,
        Sawtooth,
        Input,
        Uploaded
    };
    

    const float HI = 1.0f;
    const float LO = -1.0f;
    const float PI = 3.14159265358979323846;
    const float TWO_PI = 2.0 * PI;
    void updatePhaseDelta();
    double currentSampleRate = 0.0;
    float frequency = 200.0f;
    double phase = 0.0;
    double phaseDelta = 0.0;
    float lastHeldValue = 0.0;
    int lastHeldIndex = 0;
    float getRandomFloat();
    void bitcrushBuffer(float* buffer, int bufferSize, int bitDepth);
    void holdSampleRateReduceBuffer(float* buffer, int bufferSize, int sampleRateReduceFactor);
    void linearSampleRateReduceBuffer(float* buffer, int bufferSize, int sampleRateReduceFactor);
    void distortBuffer(float* buffer, int bufferSize, float gain, int distortionType);
    void addDelayToBuffer(float* buffer, int bufferSize, float delayTime, float delayLevel, int numRepeats, int delayType);
    
    std::vector<float> delayBuffer;
    int delayBufferIndex = 0;
    InputComponent inputComponent{};
    OutputComponent outputComponent{};
    EffectComponent effectComponent{};
    VisualizationComponent visualComponent{};



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
