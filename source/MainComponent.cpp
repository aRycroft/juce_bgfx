//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#include "MainComponent.h"

MainComponent::MainComponent(AudioProcessorValueTreeState &apvts) : apvts(apvts)
{
    addAndMakeVisible(myComp);
    setBackgroundColour(Colour(0x1F, 0x1F, 0x1F));
    startPeriodicRepaint(60);
    this->apvts.addParameterListener("pos1", this);
}

void MainComponent::paint(juce::Graphics &g)
{
    g.fillAll(Colours::black);
}

void MainComponent::resized()
{
    myComp.setBounds(rect);
}

void MainComponent::parameterChanged(const String &parameterID, float newValue)
{
    const MessageManagerLock mmLock;
    // the event loop will now be locked so it's safe to make a few calls..
    myComp.setBounds (Rectangle<int> ((int)newValue, (int)newValue,100, 100));
    myComp.repaint();
}