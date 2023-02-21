#include "math.h"
#include <JuceHeader.h>

class Grain
{
public:
    Grain() = default;
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
    void startGrain()
    {
        isPlaying = true;
        currentIndex = 0;
        phaseDelta = (float)(MathConstants<double>::twoPi * frequency / sampleRate);
    };
    void fillBuffer(AudioBuffer<float> &buffer, int numSamples)
    {
        if (isPlaying)
        {
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                buffer.addFrom(channel, 0, *audioBuffer, channel, currentIndex, numSamples);
            }
            currentIndex += numSamples;

            if (grainLengthMs / 1000 * sampleRate < currentIndex)
            {
                isPlaying = false;
            }
        }
    };

private:
    bool isPlaying{false};
    double sampleRate, grainLengthMs;
    int currentIndex{0};
    double phase, phaseDelta = 0.0;
    float frequency = 1000.0f;
    AudioBuffer<float> *audioBuffer;
};