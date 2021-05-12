/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginDay2AudioProcessorEditor::AudioPluginDay2AudioProcessorEditor (AudioPluginDay2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    cachedBgColor = audioProcessor.bgColor->get();
    setSize (400, 300);
    
    startTimerHz(20);
}

AudioPluginDay2AudioProcessorEditor::~AudioPluginDay2AudioProcessorEditor()
{
    //audioProcessor.shouldPlaySound->beginChangeGesture();
    //audioProcessor.shouldPlaySound->setValueNotifyingHost( false );
    //audioProcessor.shouldPlaySound->endChangeGesture();
    AudioPluginDay2AudioProcessor::UpdateAutomatableParameter(audioProcessor.shouldPlaySound, false);
}

void AudioPluginDay2AudioProcessorEditor::timerCallback()
{
    update();
}

void AudioPluginDay2AudioProcessorEditor::update()
{
    cachedBgColor = audioProcessor.bgColor->get();
    repaint();
}
//==============================================================================
void AudioPluginDay2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour( juce::ResizableWindow::backgroundColourId).interpolatedWith(juce::Colours::red, cachedBgColor ));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginDay2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AudioPluginDay2AudioProcessorEditor::mouseUp(const juce::MouseEvent &e)
{
    //audioProcessor.shouldPlaySound->beginChangeGesture();
    //audioProcessor.shouldPlaySound->setValueNotifyingHost( !audioProcessor.shouldPlaySound->get() );
    //audioProcessor.shouldPlaySound->endChangeGesture();
    AudioPluginDay2AudioProcessor::UpdateAutomatableParameter(audioProcessor.shouldPlaySound, !audioProcessor.shouldPlaySound->get() );
}

void AudioPluginDay2AudioProcessorEditor::mouseDown(const juce::MouseEvent &e)
{
    lastClickPos = e.getPosition();
}

void AudioPluginDay2AudioProcessorEditor::mouseDrag(const juce::MouseEvent &e)
{
    auto clickPos = e.getPosition();
    
    auto difY = juce::jlimit( -1.0, 1.0, -(clickPos.y - lastClickPos.y) / 200.0);
    DBG( "difY: " << difY );
    
    AudioPluginDay2AudioProcessor::UpdateAutomatableParameter(audioProcessor.bgColor, difY);
    update();
}
