//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#pragma once

#include <JuceHeader.h>


class MyComponent : public Component
{
public:

    MyComponent(){
        // path.addTriangle (0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f);
        // path.applyTransform (AffineTransform::rotation (MathConstants<float>::twoPi * 0, 0.5f, 0.5f));
    };
    ~MyComponent() override{

    };

    void paint (juce::Graphics& g) override{
        Path path2;
        path2.addRectangle(getLocalBounds());
        g.setColour(Colours::aliceblue);
        g.fillRect(getLocalBounds());
        g.setColour(Colours::green);
        g.fillPath(path2);
        g.setColour(Colours::red);
        g.fillEllipse(getLocalBounds().toFloat());
    };

    void resized() override{

    };

private:
    Path path;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyComponent)
};
