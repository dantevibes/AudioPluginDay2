/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "PluginProcessor.h"

//==============================================================================
/**
*/

struct AudioPluginDay2AudioProcessor;

class AudioPluginDay2AudioProcessorEditor  : public juce::AudioProcessorEditor , public juce::Timer
{
public:
    AudioPluginDay2AudioProcessorEditor (AudioPluginDay2AudioProcessor&);
    ~AudioPluginDay2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseUp( const juce::MouseEvent& e ) override;
    void mouseDown( const juce::MouseEvent& e ) override;
    void mouseDrag( const juce::MouseEvent& e ) override;
    void timerCallback() override;

private:
    void update();
    juce::Point<int> lastClickPos;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginDay2AudioProcessor& audioProcessor;
    float cachedBgColor = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginDay2AudioProcessorEditor)
};
