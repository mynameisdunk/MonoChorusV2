/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
 
 This is a test bit of text to check I've opened the correct file ( you have )
 
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DelayLine.h"
#include "BBD.h"
#include "Compander.h"

//==============================================================================
MonoChorusV2AudioProcessor::MonoChorusV2AudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                      ), params(apvts)
{
   
}

MonoChorusV2AudioProcessor::~MonoChorusV2AudioProcessor()
{
    
    inputAlias.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

//==============================================================================
const juce::String MonoChorusV2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MonoChorusV2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MonoChorusV2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MonoChorusV2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MonoChorusV2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MonoChorusV2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MonoChorusV2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void MonoChorusV2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MonoChorusV2AudioProcessor::getProgramName (int index)
{
    return {};
}

void MonoChorusV2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

juce::AudioProcessorParameter* MonoChorusV2AudioProcessor::getBypassParameter() const
{
return params.bypassParam;
}

//==============================================================================
void MonoChorusV2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    delayLine.prepare(spec);
    
    double numSamples = BBD::maxDelayTime / 1000.0f * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));
    delayLine.setMaximumDelayInSamples(maxDelayInSamples);
    delayLine.reset();
    
    sinLFO.prepare(sampleRate);
    sinLFO.reset();
    
    triLFO.prepare(sampleRate);
    sinLFO.reset();
    
// COMPANDER
    mEnvelopeFollower.prepare(sampleRate);
    mEnvelopeFollower.reset();
    mCompressor.prepare(sampleRate);
    mCompressor.reset();
    mExpander.prepare(sampleRate);
    mExpander.reset();
    
    rateParamInHz = 1.0f;
// FILTERS
    
    inputHighPassOne.prepare(spec);
    inputHighPassOne.reset();
    inputHighPassOne.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    inputHighPassOne.setCutoffFrequency(inputHPFreqOne);
    
    inputHighPassTwo.prepare(spec);
    inputHighPassTwo.reset();
    inputHighPassTwo.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    inputHighPassTwo.setCutoffFrequency(inputHPFreqTwo);
    
    inputAlias.prepare(spec);
    inputAlias.reset();
    inputAlias.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    inputAlias.setCutoffFrequency(testFreq);
    
    bandwidthFilter.prepare(spec);
    bandwidthFilter.reset();
    bandwidthFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    bandwidthFilter.setCutoffFrequency(bandwidthFreq);
    
    
}

void MonoChorusV2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MonoChorusV2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void MonoChorusV2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    rateParamInHz = params.rate;
    sinLFO.setFrequency(rateParamInHz);
    triLFO.setFrequency(rateParamInHz);
    
    float sampleRate = float(getSampleRate());
    
    float* channelDataL = buffer.getWritePointer(0);
    float* channelDataR = buffer.getWritePointer(1);
    
    scaledDepth = params.depth/ (1 + params.rate * k);
    
    params.update();
    params.smoothen();
    
    
    inputAlias.setCutoffFrequency(aliasFreq);
    
    for (int samp = 0; samp < buffer.getNumSamples(); ++samp )
        {
            /*   ============  Sine LFO - Don't delete will adapt for use later ====
            lfoValue = sinLFO.process(); // LFO position -1 to 1
            float delayOffset = (lfoValue * params.depth);
            float delayModded = (baseDelay + delayOffset);
            juce::jlimit((baseDelay - params.maxDepth), (baseDelay + params.maxDepth), delayModded);
            float delayInSamples = delayModded / 1000.0f * sampleRate;
            delayLine.setDelay(delayInSamples); */
            
            lfoValue = triLFO.process(); // LFO position -1 to 1
            float delayOffset = (lfoValue * params.depth);
            float delayModded = (baseDelay + delayOffset);
            juce::jlimit((baseDelay - params.maxDepth), (baseDelay + params.maxDepth), delayModded);
            float delayInSamples = delayModded / 1000.0f * sampleRate;
            delayLine.setDelay(delayInSamples);
            
            float bandwidthCutoff = bucketB.calcBandwidthFreq(delayModded);
            bandwidthFilter.setCutoffFrequency(bandwidthCutoff);
            
            float dryL = channelDataL[samp];
            float dryR = channelDataR[samp];
            
            dryL = inputAlias.processSample(0, dryL);
            dryR = inputAlias.processSample(1, dryR);
            
            float compDryL = mCompressor.process(dryL);
            float compDryR = mCompressor.process(dryR);
            
            delayLine.pushSample(0, compDryL);
            delayLine.pushSample(1, compDryR);
            
            float wetL = delayLine.popSample(0);
            float wetR = delayLine.popSample(1);
            
            wetL = inputFilterHP(0, wetL);
            wetR = inputFilterHP(1, wetR);
            
            wetL = bandwidthFilter.processSample(0, wetL);
            wetR = bandwidthFilter.processSample(1, wetR);
            
            wetL = mExpander.process(wetL);
            wetR = mExpander.process(wetR);
            
            dryL = inputHighPassOne.processSample(0, dryL);
            dryR = inputHighPassOne.processSample(1, dryR);
            
            float mixL = dryL + (wetL * params.mix);
            float mixR = dryR + (wetR * params.mix);
            
            if (!params.bypassed)
            {
                mixL = dryL;
                mixR = dryR;
            }
            
            channelDataL[samp] =  mixL * params.gain;
            channelDataR[samp] =  mixR * params.gain;
            
            
            
            
        }
}

//==============================================================================
bool MonoChorusV2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MonoChorusV2AudioProcessor::createEditor()
{
    return new MonoChorusV2AudioProcessorEditor (*this);
}

//==============================================================================
void MonoChorusV2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void MonoChorusV2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MonoChorusV2AudioProcessor();
}

