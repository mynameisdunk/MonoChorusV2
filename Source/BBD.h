#pragma once
#include <JuceHeader.h>

class BBD
{
    public:
     
// VARIABLES
    
// DELAY LINE VARIABELS
    static constexpr float minDelayTime = 20.5f;
    static constexpr float maxDelayTime = 50.0f; // These values represent the normally used range of delay time in a BBD chorus
    
// GETTERS
    int getStages() const noexcept
    {
        return stages;
    }
    
    float getBaseDelayTime() const noexcept
    {
        return baseDelayTime;
    }
    
    
    float getInputHighPassOne() const noexcept
    {
        return inputHighPassOne;
    }
    
    float getInputHighPassTwo () const noexcept
    {
        return inputHighPassTwo;
    }
    
    float getAntiAliasFreq() const noexcept
    {
        return antiAliasFreq;
    }
    
    float calcClockFreq(float currentDelayTime)
    {
        float cCF = getStages() / currentDelayTime;
        return cCF;
    }
    
    float calcBandwidthFreq(float currentDelayTime)
    {
        float cCF = getStages() / currentDelayTime;
        float bandwidthFreq = (cCF / 3.0f) * 1000.0f;
        bandwidthFreq = 18000.0f * std::tanh(bandwidthFreq / 18000.0f);
        return bandwidthFreq;
    }
    
    
    private:
    
    int stages = 1024; // Hardcoded number of stages in hypothetical chip HIDDEN INFORMATION
    float baseDelayTime = 24.0f; // This value can be changed between the public variable values (although not strictly enforced will probably throw an error if it does exceed maxDelayTime..) EXPERIMENT to find the best setting. Perhaps a switch down the line for a couple different sounds?
    
    // This value needs to be altered instead by the modulated delay time probably with some heavy skewing to keep the
    
    float inputHighPassOne = 120.0f;
    float inputHighPassTwo = 250.0f;
    
    float antiAliasFreq = 18000.0f;
};
