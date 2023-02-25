//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#include "MainComponent.h"

MainComponent::MainComponent(MyPluginAudioProcessor &p, AudioProcessorValueTreeState &apvts) : audioProcessor(p), apvts(apvts),
                                                                                               myComp([&](Rectangle<int> rect)
                                                                                                      { audioProcessor.startGrain(rect, 0.0); })
{
    this->apvts.addParameterListener("pos1", this);
    addMouseListener(this, true);
    for(auto* grain : audioProcessor.grains)
    {
        addAndMakeVisible(grain);
    }
    setOpaque(true);
    setBackgroundColour(Colour(0x5F, 0x1F, 0x1F));
    enableRenderStats();
    startPeriodicRepaint();
}

void MainComponent::paint(juce::Graphics &g)
{
    //g.fillAll(Colours::black);
}

void MainComponent::resized()
{
    //myComp.setBounds(getLocalBounds());
}

void MainComponent::parameterChanged(const String &parameterID, float newValue)
{
    const MessageManagerLock mmLock;
    // the event loop will now be locked so it's safe to make a few calls..
    // myComp.setBounds (Rectangle<int> ((int)newValue, (int)newValue,100, 100));
    // myComp.repaint();
}

void MainComponent::mouseDown(const MouseEvent &event)
{
    DBG("MOUSE EVENT ----- " << event.x << " " << event.y);
    audioProcessor.startGrain(Rectangle<int>(event.x, event.y, 100, 100), (double) event.x / 600);
}