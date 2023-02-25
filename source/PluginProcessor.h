/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Grain.h"

//==============================================================================
/**
*/
class MyPluginAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MyPluginAudioProcessor();
    ~MyPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    void startGrain(Rectangle<int> bounds, double startPosition);
    
    OwnedArray<Grain> grains;
private:
    //==============================================================================

    AudioProcessorValueTreeState::ParameterLayout createParameterLayout (){
      return AudioProcessorValueTreeState::ParameterLayout{
        std::make_unique<AudioParameterFloat>("frequency", "Frequency", NormalisableRange<float>(20.0f, 20000.0f, 0.1f, 0.5f), 5000.0f),
        std::make_unique<AudioParameterFloat>("amplitude", "Amplitude", NormalisableRange<float>(0.0f, 1.0f, 0.01f, 0.5f), 0.2f),
        std::make_unique<AudioParameterFloat>("pos1", "Position1", 0.0f, 1000.0f, 0.0f),
      };
    }
    int counter{0};
    int currentGrainIndex{0};
    Grain testGrain;
    AudioSampleBuffer audioBuffer;
    AudioProcessorValueTreeState apvts;
    AudioFormatManager formatManager;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyPluginAudioProcessor)
};
