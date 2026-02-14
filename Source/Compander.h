#pragma once
#include <JuceHeader.h>

class EnvelopeFollower
{
public:
    EnvelopeFollower ();
    
    void prepare (double sampleRate);
    float processSample (float input);
    void setAttackTime(float timeMs);
    void setReleaseTime(float timeMs);
    void reset ();
    
private:
    
    float mEnvelope;
    float mAttackCoeff; // amount of old value to keep
    float mReleaseCoeff; // amount of old value to keep
    double mSampleRate;
    
};


class Compressor
{
public:
    Compressor();
    
    void prepare (double sampleRate);
    void setThreshold (float threshold_dB); // While we are initialising these with values already, it allows us to adapt this to a user changeable variable later
    void setRatio (float ratio);
    void setAttackTime (float timeMs);
    void setReleaseTime (float timeMs);
    float process(float inputSample); // this is where the compression happens
    void reset();
   
    
private:
    
    EnvelopeFollower mEnvelopeFollower;
    
    float mThreshold_dB;
    float mRatio;
    float mAttackTime = 1.0f;
    float mReleaseTime = 50.0f;
    
    float linearTodB(float linear)
    {
        if (linear <= 0.0f)
            return -100.0f;
        
        return 20.0f * std::log10(linear);
    }
    
    float dBToLinear(float dB)
    {
        return std::pow(10.0f, dB / 20.0f);
    }
    
    
};



class Expander
{
public:
    Expander();
   
    void prepare(double sampleRate);
    void setThreshold (float threshold_dB);
    void setRatio (float ratio);
    void setAttackTime (float timeMs);
    void setReleaseTime (float timeMs);
    float process(float inputSample);
    void reset();
   
private:
    EnvelopeFollower mEnvelopeFollower;
    
    float mThreshold_dB;
    float mRatio;
    
    float linearTodB(float linear)
    {
        if (linear <= 0.0f)
            return -100.0f;
        
        return 20.0f * std::log10(linear);
        
    }
    
    float dBToLinear(float dB)
    {
        return std::pow(10.0f, dB / 20.0f);
    }
};
