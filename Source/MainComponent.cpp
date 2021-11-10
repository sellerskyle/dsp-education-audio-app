#include "MainComponent.h"



//==============================================================================
MainComponent::MainComponent() 
{
    // Make sure you set the size of the component after
    // you add any child components.

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [this](bool granted)
        {
            int numInputChannels = granted ? 2 : 0;
            setAudioChannels(numInputChannels, 2);
        });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }


    delayBuffer = std::vector<float>(3 * ANTICIPATED_SAMPLE_RATE, 0.0f);

    addAndMakeVisible(inputComponent);
    addAndMakeVisible(outputComponent);
    addAndMakeVisible(effectComponent);
    addAndMakeVisible(visualComponent);
    

    setSize(800, 600);
    setSize(1000, 600);

    srand(time(NULL));
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    inputComponent.transportSource.setSource(nullptr);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    currentSampleRate = sampleRate;
    effectComponent.setSampleRate(sampleRate);

    delayBuffer.resize(3 * sampleRate);

    updatePhaseDelta();
    inputComponent.transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    
    auto inputLevel = inputComponent.getInputVolume();
    auto outputLevel = outputComponent.getOutputVolume();
    frequency = inputComponent.getFrequency();
    updatePhaseDelta();
    visualComponent.setSignalFrequency(frequency);

    auto* leftChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannelData = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    const auto* inputData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        
    auto* device = deviceManager.getCurrentAudioDevice();
    auto activeInputChannels = device->getActiveInputChannels();
    //Selecting Input
    if (inputComponent.getInputType() == 7) // input file
    {

        if (inputComponent.readerSource.get() == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();

        }
        else
        {
            inputComponent.transportSource.getNextAudioBlock(bufferToFill);
        }

            


    }
    else
    {
        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            float currentSample, sign, diff;
            //Selecting what wave to generate
            switch (inputComponent.getInputType())
            {
            case 1: //sine
                currentSample = (float)std::sin(phase);
                break;
            case 2: //square
                sign = std::sinf(phase);
                currentSample = sign >= 0 ? HI : LO;
                break;
            case 3: //Triangle
                sign = fmodf(phase, TWO_PI);
                diff = fmodf(phase, PI) / PI;
                currentSample = 2.0 * diff - 1.0;
                if (sign < PI)
                {
                    currentSample = currentSample * -1;
                }

                break;
            case 4: //Sawtooth
                diff = fmodf(phase, TWO_PI) / TWO_PI;
                currentSample = 2.0 * diff - 1.0;
                break;
            case 5: //Noise
                currentSample = getRandomFloat();
                break;
            case 6: //Input
                currentSample = inputData[i];
                break;
            case 7: //File (already filled above)
                break;


                inputComponent.transportSource.getNextAudioBlock(bufferToFill);
                currentSample = 0.0f;
                break;
            default:
                currentSample = 0.0f;
                break;
            }

            phase += phaseDelta;
            leftChannelData[i] = currentSample * inputLevel;
            rightChannelData[i] = leftChannelData[i];
        }
    }


        

    //Selecting Effect
    switch (effectComponent.getEffectType())
    {
        case 1: // distortion
            distortBuffer(leftChannelData, bufferToFill.numSamples, effectComponent.getDistortionGain(), effectComponent.getDistortionType());
            distortBuffer(rightChannelData, bufferToFill.numSamples, effectComponent.getDistortionGain(), effectComponent.getDistortionType());
            break;
        case 2: //bitcrusher
            bitcrushBuffer(leftChannelData, bufferToFill.numSamples, effectComponent.getBitDepth());
            bitcrushBuffer(rightChannelData, bufferToFill.numSamples, effectComponent.getBitDepth());
            holdSampleRateReduceBuffer(leftChannelData, bufferToFill.numSamples, effectComponent.getSampleRateReductionFactor());
            holdSampleRateReduceBuffer(rightChannelData, bufferToFill.numSamples, effectComponent.getSampleRateReductionFactor());
            break;
        case 3:
            addDelayToBuffer(leftChannelData, bufferToFill.numSamples, effectComponent.getDelayTime(), effectComponent.getDelayLevel(), effectComponent.getNumRepeats(), effectComponent.getDelayType());
        default:
            break;
    }

        
    visualComponent.pushBuffer(*bufferToFill.buffer);
    //reducing volume by output Level
    for (auto i = 0; i < bufferToFill.numSamples; ++i)
    {
        visualComponent.pushNextSampleIntoFifo(leftChannelData[i]); //for spectrogram and spectrum analyzer
        leftChannelData[i] *= outputLevel;
        rightChannelData[i] = leftChannelData[i];
    }


}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    inputComponent.transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::lightgreen);
    inputComponent.setBounds(0, 0, int(0.1 * getParentWidth()), getParentHeight());
    effectComponent.setBounds(int(0.1 * getParentWidth()), 0, int(0.1 * getParentWidth()), getParentHeight());
    visualComponent.setBounds(int(0.2 * getParentWidth()), 0, int(0.7 * getParentWidth()), getParentHeight());
    outputComponent.setBounds(int(0.9 * getParentWidth()), 0, int(0.1 * getParentWidth()), getParentHeight());

}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    inputComponent.setBounds(0, 0, int(0.1 * getParentWidth()), getParentHeight());
    effectComponent.setBounds(int(0.1 * getParentWidth()), 0, int(0.1 * getParentWidth()), getParentHeight());
    visualComponent.setBounds(int(0.2 * getParentWidth()), 0, int(0.7 * getParentWidth()), getParentHeight());
    outputComponent.setBounds(int(0.9 * getParentWidth()), 0, int(0.1 * getParentWidth()), getParentHeight());

}

float MainComponent::getRandomFloat()
{
    return  LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

void MainComponent::bitcrushBuffer(float* buffer, int bufferSize, int bitDepth)
{
    float totalQLevels = powf(2, bitDepth);
    float val, remainder;
    for (int i = 0; i < bufferSize; i++)
    {
        val = buffer[i];
        remainder = fmodf(val, 1 / totalQLevels);

        buffer[i] = val - remainder;
    }

}

void MainComponent::holdSampleRateReduceBuffer(float* buffer, int bufferSize, int sampleRateReduceFactor)
{
    if (sampleRateReduceFactor > 1)
    {
        for (int i = 0; i < bufferSize; i++)
        {
            if (lastHeldIndex >= sampleRateReduceFactor)
            {
                lastHeldValue = buffer[i];
                lastHeldIndex = 0;
            }
            buffer[i] = lastHeldValue;
            lastHeldIndex++;
        }
    }

}

void MainComponent::linearSampleRateReduceBuffer(float* buffer, int bufferSize, int sampleRateReduceFactor)
{
    int sampleNumber;
    if (sampleRateReduceFactor > 1)
    {
        for (int i = 0; i < bufferSize-sampleRateReduceFactor; i++)
        {
            sampleNumber = i % sampleRateReduceFactor;
            if (sampleNumber != 0) 
                buffer[i] = buffer[i - (sampleNumber)] + (sampleNumber * ((buffer[i + (sampleRateReduceFactor - sampleNumber)] - buffer[i - (sampleNumber)]) / sampleRateReduceFactor));
        }
    }
}

void MainComponent::distortBuffer(float* buffer, int bufferSize, float gain, int distortionType)
{
    if (distortionType == 1) // hard clip
    {
        for (int i = 0; i < bufferSize; i++)
        {
            buffer[i] = buffer[i] * gain;
            if (buffer[i] >= 1.0)
                buffer[i] = 1.0;
            if (buffer[i] <= -1.0) 
                buffer[i] = -1.0;
        }
    }
    else if (distortionType == 2)//tanh
    {
        for (int i = 0; i < bufferSize; i++)
        {
            buffer[i] = tanh(buffer[i] * gain);
        }
    }

}

void MainComponent::addDelayToBuffer(float* buffer, int bufferSize, float delayTime, float delayLevel, int numRepeats, int delayType)
{
    int delaySamples = (int)floor(delayTime * (currentSampleRate - 1)) + 1;
    int delayIndex;
    float temp;

    if (delayType == 1) // FIR
    {
        for (int i = 0; i < bufferSize; i++)
        {
            temp = buffer[i];
            buffer[i] = 0.0f;
            //FIR
            delayIndex = delayBufferIndex - delaySamples;
            for (int j = 0; j < numRepeats; j++)
            {
                if (delayIndex < 0)
                {
                    delayIndex += delayBuffer.size();
                }
                buffer[i] += delayBuffer[delayIndex] * delayLevel * 0.5;
                delayIndex -= delaySamples;
            }

            buffer[i] += 0.5 * temp;
            delayBuffer[delayBufferIndex] = temp;
            


            delayBufferIndex = (delayBufferIndex + 1) % delayBuffer.size();
        }
    }
    else // IIR
    {
        for (int i = 0; i < bufferSize; i++)
        {
            //IIR
            delayIndex = delayBufferIndex - delaySamples;
            if (delayIndex < 0)
            {
                delayIndex += delayBuffer.size();
            }
            buffer[i] = 0.5 * buffer[i];
            buffer[i] += delayBuffer[delayIndex] * delayLevel * 0.5;
            delayBuffer[delayBufferIndex] = buffer[i];

            delayBufferIndex = (delayBufferIndex + 1) % delayBuffer.size();
        }
    }


}

void MainComponent::updatePhaseDelta()
{
    auto cyclesPerSample = frequency / currentSampleRate;         // [2]
    phaseDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // [3]
}