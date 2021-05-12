/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginDay2AudioProcessor::AudioPluginDay2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
apvts(*this, nullptr)
{
    //shouldPlaySound = new juce::AudioParameterBool( "ShouldPlaySoundParam", "shouldPlaySound", false );
    //addParameter(shouldPlaySound);
    auto shouldPlaySoundParam = std::make_unique<juce::AudioParameterBool>("ShouldPlaySoundParam", "shouldPlaySound", false );
    
    auto* param = apvts.createAndAddParameter(std::move(shouldPlaySoundParam) );
    
    shouldPlaySound = dynamic_cast<juce::AudioParameterBool*>( param );
    
    auto bgColorParam = std::make_unique<juce::AudioParameterFloat>("Background color", "background color", 0.f, 1.f, 0.5f);
    param = apvts.createAndAddParameter(std::move(bgColorParam) );
    bgColor = dynamic_cast<juce::AudioParameterFloat*>(param);
    
    apvts.state = juce::ValueTree("PFMSynthValueTree");
}

AudioPluginDay2AudioProcessor::~AudioPluginDay2AudioProcessor()
{ }

//==============================================================================
const juce::String AudioPluginDay2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginDay2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginDay2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginDay2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginDay2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginDay2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginDay2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginDay2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioPluginDay2AudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPluginDay2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioPluginDay2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AudioPluginDay2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPluginDay2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudioPluginDay2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    juce::Random r;
    
    for (int i = 0; i < buffer.getNumSamples(); ++i )
    {
        for( int channel = 0; channel < buffer.getNumChannels(); ++channel )
        {
            if( shouldPlaySound->get() )
            {
                buffer.setSample(channel, i, r.nextFloat() * .1 );
            }
            else
            {
                buffer.setSample(channel, i, 0);
            }
        }
    }
}

//==============================================================================
bool AudioPluginDay2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginDay2AudioProcessor::createEditor()
{
    return new AudioPluginDay2AudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginDay2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, false);
    apvts.state.writeToStream( mos );
}

void AudioPluginDay2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if( tree.isValid() )
    {
        apvts.state = tree;
    }
    DBG( apvts.state.toXmlString() );
}

void AudioPluginDay2AudioProcessor::UpdateAutomatableParameter(juce::RangedAudioParameter* param, float value)
{
    param->beginChangeGesture();
    param->setValueNotifyingHost(value);
    param->endChangeGesture();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginDay2AudioProcessor();
}
