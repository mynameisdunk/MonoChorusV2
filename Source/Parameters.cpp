#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                          const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); // parameter does not exist or wrong type
}

static juce::String stringFromMilliseconds( float value, int)
{
    if (value < 10.0f)
    {
        return juce::String(value, 2) + "ms";
    }
    else if (value < 100.0f)
    {
        return juce::String(value, 1) + "ms";
    }
    else if (value < 1000.0f)
    {
        return juce::String(int(value)) + "ms";
    }
    else
    {
        return juce::String(value*0.001, 2) + "s";
    }
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + "dB";
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, rateParamID, rateParam);
    castParameter(apvts, depthParamID, depthParam);
    castParameter(apvts, mixParamID, mixParam);
    castParameter(apvts, bypassParamID, bypassParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(gainParamID, "Output Gain", juce::NormalisableRange<float> {-12.0f, 12.0f}, 0, juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)));;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(rateParamID, "Rate", juce::NormalisableRange<float> {minRate, maxRate, 0.01f}, 1.2f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(depthParamID, "Depth", juce::NormalisableRange<float> {minDepth, maxDepth, 0.01f}, 2.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(mixParamID, "Mix", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 0.8f));
    layout.add(std::make_unique<juce::AudioParameterBool>(bypassParamID, "Bypass", false));
    
    return layout;
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    rateSmoother.setTargetValue(rateParam->get());
    depthSmoother.setTargetValue(depthParam->get());
    mixSmoother.setTargetValue(mixParam->get());
    bypassParam->get();
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.002f;
    gainSmoother.reset(sampleRate, duration);
    rateSmoother.reset(sampleRate, duration);
    depthSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
   
}

void Parameters::reset() noexcept
{
    gain = 1.0f;
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    
    rate = 1.0f;
    rateSmoother.setCurrentAndTargetValue(rateParam->get());
               
    depth = 0.1f;
    depthSmoother.setCurrentAndTargetValue(depthParam->get());
    
    mix = 0.8f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get());
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    rate = rateSmoother.getNextValue();
    depth = depthSmoother.getNextValue();
    mix = mixSmoother.getNextValue();
}
