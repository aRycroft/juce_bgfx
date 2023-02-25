#include "math.h"
#include <JuceHeader.h>

class Grain : public Component
{
public:
    Grain(){
        setInterceptsMouseClicks(false, false);
    };
    void setSampleRate(double sampleRate)
    {
        this->sampleRate = sampleRate;
    };
    void setGrainLength(double grainLengthMs)
    {
        this->grainLengthMs = grainLengthMs;
    };
    void setAudioBuffer(AudioBuffer<float> *audioBuffer)
    {
        this->audioBuffer = audioBuffer;
    };
    void startGrain(Rectangle<int> bounds, double startPosition)
    {
        isPlaying = true;
        startIndex = currentIndex = audioBuffer->getNumSamples() * startPosition;
        finalIndex = std::min((double) startIndex + grainLengthMs / 1000 * sampleRate, (double)audioBuffer->getNumSamples());
        startAnimation(bounds);
    };
    void fillBuffer(AudioBuffer<float> &buffer, int numSamples)
    {
        if (isPlaying)
        {
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto samplesToAdd = std::min(numSamples, finalIndex - currentIndex);
                buffer.addFrom(channel, 0, *audioBuffer, channel, currentIndex, samplesToAdd);
            }

            currentIndex += numSamples;

            if (currentIndex >= finalIndex)
            {
                isPlaying = false;
            }
        }
    };
    //-------/------- GUI -------/-------//
    void paint(juce::Graphics &g) override
    {
        Path path2;
        path2.addRectangle(getLocalBounds());
        g.setColour(Colours::aliceblue);
        g.fillRect(getLocalBounds());
        g.setColour(Colours::green);
        g.fillPath(path2);
        g.setColour(Colours::red);
        g.fillEllipse(getLocalBounds().toFloat());
    };

    void resized() override
    {
    };

private:
    void startAnimation(Rectangle<int> bounds)
    {
        rect = bounds.withX(bounds.getX() - 50).withY(bounds.getY() - 50);
        setAlpha(1.0f);
        setBounds(rect);
        resized();
        Desktop::getInstance().getAnimator().animateComponent(this, rect.expanded(100, 100), 0.0f, 5000, false, 1.0, 0.0);
    }
    bool isPlaying{false};
    double sampleRate, grainLengthMs;
    int currentIndex{0}, startIndex{0}, finalIndex{0};
    AudioBuffer<float> *audioBuffer;
    Rectangle<int> rect;
};