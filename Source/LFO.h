#pragma once
#include <JuceHeader.h>

class sineLFO
{
public:
    sineLFO()
    {
        mPhase = 0.0f;
        mIncrement = 0.0f;
        mFrequency = 1000.0f; // LFO frequency also known as RATE - set by setFreq function. Parameter value passed as argument
        mSampleRate = 44100.0f;
    }
    
    static constexpr float twoPi = juce::MathConstants<float>::twoPi;
    
    void prepare(float sampleRate)
    {
        mSampleRate = sampleRate;
        
        calculateIncrement();
    }
    
    void setFrequency (float frequencyHz)
    {
        mFrequency = frequencyHz;
        calculateIncrement();
    }
    
    float process() // Literally the LFO code 
    {
        float sine = std::sin(mPhase * twoPi);
        mPhase += mIncrement;
        
        if (mPhase >= 1.0f)
        {
            mPhase -= 1.0f;
        }
        
        return sine;  
    }
    
    void reset()
    {
        mPhase = 0.0f;
    }
    
private:
    
    float mPhase;
    float mIncrement;
    float mFrequency;
    float mSampleRate;
    
    void calculateIncrement()
    {
        mIncrement = mFrequency / mSampleRate;
    }

};

//=====================================================================================================================================================================================

class triangleLFO
{
public:
    triangleLFO()
    {
        mPhase = 0.0f;
        mIncrement = 0.0f;
        mFrequency = 1000.0f; // LFO frequency also known as RATE - set by setFreq function. Parameter value passed as argument
        mSampleRate = 44100.0f;
    }
    
    static constexpr float twoPi = juce::MathConstants<float>::twoPi;
    
    void prepare(float sampleRate)
    {
        mSampleRate = sampleRate;
        
        calculateIncrement();
    }
    
    void setFrequency (float frequencyHz)
    {
        mFrequency = frequencyHz;
        calculateIncrement();
    }
    
    float process() // Literally the LFO code
    {
        float triangle = 2.0f * std::abs(2.0f * mPhase - 1.0f) - 1.0f;

        mPhase += mIncrement;
        
        float skew = skewAmount;
        
        float skewedTri = std::copysign(std::pow(std::abs(triangle), skew), triangle);
        skewedTri = juce::jlimit(-1.0f, 1.0f, skewedTri);
        
        if (mPhase >= 1.0f)
        {
            mPhase -= 1.0f;
        }
        
        return skewedTri;
    }
    
    void reset()
    {
        mPhase = 0.0f;
    }
    
private:
    
    float mPhase;
    float mIncrement;
    float mFrequency;
    float mSampleRate;
    float skewAmount = 1.4f;
    
    void calculateIncrement()
    {
        mIncrement = mFrequency / mSampleRate;
    }

};



