//
//  Copyright (C) 2022 Arthur Benilov <arthur.benilov@gmail.com>
//

#pragma once

#include <JuceHeader.h>

class MyComponent : public Component
{
public:
    MyComponent(std::function<void(Rectangle<int>)> startGrain) : startGrain(startGrain){
                                                                      // path.addTriangle (0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f);
                                                                      // path.applyTransform (AffineTransform::rotation (MathConstants<float>::twoPi * 0, 0.5f, 0.5f));
                                                                      // addMouseListener(this, true);
                                                                      this->setInterceptsMouseClicks(false, false);
                                                                  };
    ~MyComponent() override{

    };

    void paint(juce::Graphics &g) override
    {
        g.fillAll(Colours::white);
    };

    void resized() override{

    };

    void mouseDown(const MouseEvent &e) override{
 // dragger.startDraggingComponent (this, e);
        // if (e.mods.isLeftButtonDown())
        // {
        //     DBG("start MOUSE EVENT ----- " << e.x << " " << e.y);
        //     startGrain(Rectangle<int>(e.x, e.y, 100, 100));
        // }
    };

    void mouseDrag(const MouseEvent &e) override{};

    void mouseUp(const MouseEvent &e) override
    {
       
    };

private:
    // ComponentDragger dragger;
    Path path;
    std::function<void(Rectangle<int>)> startGrain;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyComponent)
};
