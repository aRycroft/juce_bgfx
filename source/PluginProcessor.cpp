/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyPluginAudioProcessor::MyPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         )
#endif
      ,
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    formatManager.registerBasicFormats();
    for (int i{0}; i < 10; i++)
    {
        grains.add(new Grain());
    }
}

MyPluginAudioProcessor::~MyPluginAudioProcessor()
{
}

//==============================================================================
const juce::String MyPluginAudioProcessor::getName() const
{
    return "MyPlugin";
}

bool MyPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MyPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MyPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MyPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int MyPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyPluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MyPluginAudioProcessor::getProgramName(int index)
{
    return {};
}

void MyPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

//==============================================================================
void MyPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    auto file = File("~/Music/Music/Media.localized/Music/Unknown Artist/Unknown Album/647498__josefpres__piano-loops-014-octave-down-long-loop-120-bpm.wav");
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    AudioSampleBuffer tempBuffer;
    tempBuffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
    reader->read(&tempBuffer,                  // [5]
                 0,                            //  [5.1]
                 (int)reader->lengthInSamples, //  [5.2]
                 0,                            //  [5.3]
                 true,                         //  [5.4]
                 true);

    if (reader->sampleRate != sampleRate)
    {
        audioBuffer.setSize((int)reader->numChannels, ceil(sampleRate * reader->lengthInSamples / reader->sampleRate));

        for (int i{0}; i < audioBuffer.getNumChannels(); i++)
        {
            LinearInterpolator interpolator;
            interpolator.process(reader->sampleRate / sampleRate, tempBuffer.getReadPointer(i), audioBuffer.getWritePointer(i), audioBuffer.getNumSamples());
        }
    }
    else
    {
        audioBuffer.makeCopyOf(tempBuffer);
    }

    for (auto grain : grains)
    {
        grain->setSampleRate(sampleRate);
        grain->setGrainLength(5000);
        grain->setAudioBuffer(&audioBuffer);
    }
}

void MyPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void MyPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (auto grain : grains)
    {
        grain->fillBuffer(buffer, buffer.getNumSamples());
    }
    // ..do something to the data...
}

//==============================================================================
bool MyPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *MyPluginAudioProcessor::createEditor()
{
    return new MyPluginAudioProcessorEditor(*this, apvts);
}

//==============================================================================
void MyPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MyPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
void MyPluginAudioProcessor::startGrain(Rectangle<int> bounds, double startPosition)
{
    grains.getUnchecked(currentGrainIndex++ % grains.size())->startGrain(bounds, startPosition);
}

//==============================================================================

// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new MyPluginAudioProcessor();
}
