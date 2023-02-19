//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(600, 400);
    setAudioChannels(0, 2);

    addAndMakeVisible(myComp);

    setBackgroundColour(Colour(0x1F, 0x1F, 0x1F));
    startPeriodicRepaint(60);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(Colours::black);
}

void MainComponent::resized()
{
    auto bounds{getLocalBounds()};
    myComp.setBounds(bounds);
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double newSampleRate)
{
    sampleRate = newSampleRate;
    expectedSamplesPerBlock = samplesPerBlockExpected;
    phaseDelta = (float) (MathConstants<double>::twoPi * frequency / sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    auto originalPhase = phase;

    for (auto chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
    {
        phase = originalPhase;

        auto *channelData = bufferToFill.buffer->getWritePointer(chan, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            channelData[i] = 0.5 * std::sin(phase);

            // increment the phase step for the next sample
            phase = std::fmod(phase + phaseDelta, MathConstants<float>::twoPi);
        }
    }
}

void MainComponent::releaseResources()
{
}