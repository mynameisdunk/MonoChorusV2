/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"
#include "BBD.h"
#include "LFO.h"
#include "Compander.h"

//==============================================================================
/**
*/
class MonoChorusV2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MonoChorusV2AudioProcessor();
    ~MonoChorusV2AudioProcessor() override;

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
    
    float inputFilterHP (int channel, float inputSample)
    {
        float x = inputSample;
        x = inputHighPassOne.processSample(channel, x);
        // x = inputHighPassTwo.processSample(channel, x);
        
        return x;
    }
    
    juce::AudioProcessorValueTreeState apvts {
        *this, nullptr, "Parameters", Parameters::createParameterLayout()
    };
     
    juce::AudioProcessorParameter* getBypassParameter() const override;
    

private:
    
    
    
// INSTANCES
    Parameters params;
    BBD bucketB;
    sineLFO sinLFO;
    triangleLFO triLFO;
    EnvelopeFollower mEnvelopeFollower;
    Compressor mCompressor;
    Expander mExpander;
    
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    
    juce::dsp::StateVariableTPTFilter<float> inputHighPassOne;
    float inputHPFreqOne = bucketB.getInputHighPassOne();
    
    juce::dsp::StateVariableTPTFilter<float> inputHighPassTwo;
    float inputHPFreqTwo = bucketB.getInputHighPassTwo();
    
    juce::dsp::StateVariableTPTFilter<float> inputAlias;
    float aliasFreq = bucketB.getAntiAliasFreq();
    float testFreq = 20000.0f;
    
    juce::dsp::StateVariableTPTFilter<float> bandwidthFilter;
    float bandwidthFreq = aliasFreq;
    
    float rateParamInHz = 1.0f;
    float lfoValue = 0.0f;
    
    float baseDelay = bucketB.getBaseDelayTime();
    
    float scaledDepth; 
    float lastScaledDepth;
    float k = std::tanh(params.rate);
    
    float clockFrequency = bucketB.getAntiAliasFreq();
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonoChorusV2AudioProcessor)
};
