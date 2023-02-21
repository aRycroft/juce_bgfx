//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#pragma once

#include <JuceHeader.h>

#include "NanovgComponent.h"
#include "MyComponent.h"

class MainComponent : public NanovgComponent, AudioProcessorValueTreeState::Listener
{
public:

    MainComponent(AudioProcessorValueTreeState& apvts);

    void paint (juce::Graphics&) override;
    void resized() override;
    void parameterChanged (const String& parameterID, float newValue) override;

    // void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    // void releaseResources() override;
    // void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

private:
    MyComponent myComp;
    float phase       = 0.0f;
    float phaseDelta  = 0.0f;
    float frequency   = 5000.0f;
    float amplitude   = 0.2f;

    double sampleRate = 0.0;
    int expectedSamplesPerBlock = 0;
    int counter = 0;
    Point<float> lastMousePosition;

    Rectangle<int> rect{0, 0, 100, 100};

    AudioProcessorValueTreeState& apvts;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
