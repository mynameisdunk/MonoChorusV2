#pragma once
#include <JuceHeader.h>

const juce::ParameterID gainParamID {"gain", 1};
const juce::ParameterID rateParamID {"rate", 1};
const juce::ParameterID depthParamID {"depth", 1};
const juce::ParameterID mixParamID {"mix", 1};
const juce::ParameterID bypassParamID {"bypass", 1};

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update() noexcept;
    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    
//PARAMETER INITIALISERS
    float gain = 1.0f;
    float rate = 0.5f;
    float depth = 1.0f;
    float mix = 0.5f;
    bool bypassed = false;
    
//STATICS AND CONSTANTS (whatever that means)
    
    static constexpr float minRate = 0.1f;
    static constexpr float maxRate = 3.5f;
    
    static constexpr float minDepth = 0.01f;
    static constexpr float maxDepth = 2.0f;
    
    juce::AudioParameterBool* bypassParam;
    
private:
    
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;
    
    juce::AudioParameterFloat* rateParam;
    juce::LinearSmoothedValue<float> rateSmoother;
    
    juce::AudioParameterFloat* depthParam;
    juce::LinearSmoothedValue<float> depthSmoother;
    
    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;
    
    
    
    
    
    
};
