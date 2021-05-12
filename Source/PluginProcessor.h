/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
/*
TODO: Click anywhere on the window to play a note.
-if you click and drag, change the pitch of the note
-should we play a sound?

-save plugin state when exiting DAW
-load plugin state when loading a session
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class AudioPluginDay2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginDay2AudioProcessor();
    ~AudioPluginDay2AudioProcessor() override;

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
    juce::AudioParameterBool* shouldPlaySound = nullptr; //= false;
    juce::AudioParameterFloat* bgColor = nullptr;
    
    static void UpdateAutomatableParameter(juce::RangedAudioParameter*, float value);

private:
    juce::AudioProcessorValueTreeState apvts;
    //Random r;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginDay2AudioProcessor)
};
