#include "Compander.h"
// ==================================================================//  ENVELOPE FOLLOWER  //==================================================================
EnvelopeFollower::EnvelopeFollower ()
{
    mEnvelope = 0.0f;
    mAttackCoeff = 0.0f;
    mReleaseCoeff = 0.0f;
    mSampleRate = 44100.0;
}

void EnvelopeFollower::prepare (double sampleRate)
{
    mSampleRate = sampleRate;
    setAttackTime(5.0f);
    setReleaseTime(100.0f);
}

float EnvelopeFollower::processSample (float input)
{
    float absSample = std::abs(input);
    // float attack = mAttackCoeff; OPTIONAL TO MAKE THE NEXT BIT EASIER TO READ BUT NOT REALLY NEEDED
    // float release = mReleaseCoeff;   ^^
    
    if (absSample > mEnvelope)
    {
        mEnvelope = mAttackCoeff * mEnvelope + (1 - mAttackCoeff) * absSample;
    }
    else
    {
        mEnvelope = mReleaseCoeff * mEnvelope + (1 - mReleaseCoeff) * absSample;
    }
    
    return mEnvelope;
}

void EnvelopeFollower::setAttackTime(float timeMs)
{
    float timeInSeconds = timeMs / 1000.0f;
    mAttackCoeff = std::exp (-1 / (timeInSeconds * mSampleRate));
    
} // I don't see any reason not to hard code these values ^ v
  // It does make it slightly more reusable I guess?

void EnvelopeFollower::setReleaseTime(float timeMs)
{
    float timeInSeconds = timeMs / 1000.0f;
    mReleaseCoeff = std::exp (-1 / (timeInSeconds * mSampleRate));
    
}

void EnvelopeFollower::reset ()
{
    mEnvelope = 0.0f;
}

// ==================================================================//  COMPRESSOR  //==================================================================
Compressor::Compressor()
{
    mThreshold_dB = -20.0f;
    mRatio = 4.0f;
}

void Compressor::prepare (double sampleRate)
{
    mEnvelopeFollower.prepare(sampleRate);
    mEnvelopeFollower.setAttackTime(mAttackTime);
    mEnvelopeFollower.setReleaseTime(mReleaseTime);
    setThreshold(mThreshold_dB);
    setRatio(mRatio);
   
}

void Compressor::setThreshold (float threshold_dB) // While we are initialising these with values already, it allows us to adapt this to a user changeable variable later
{
    mThreshold_dB = threshold_dB;
}

void Compressor::setRatio (float ratio)
{
    mRatio = ratio;
}

void Compressor::setAttackTime (float timeMs)
{
    mEnvelopeFollower.setAttackTime(timeMs);
}

void Compressor::setReleaseTime (float timeMs)
{
    mEnvelopeFollower.setReleaseTime(timeMs);
}

float Compressor::process(float inputSample) // this is where the compression happens
{
    float envelope = mEnvelopeFollower.processSample(inputSample);
    
    float envelope_dB = linearTodB(envelope);
    
    float gainReduction_dB = 0.0f;
    
    if (envelope_dB > mThreshold_dB)
    {
        float overShoot_dB = envelope_dB - mThreshold_dB;
        gainReduction_dB = overShoot_dB * (1.0f - 1.0f /mRatio);
    }
    
    float gainReductionLinear = dBToLinear(-gainReduction_dB);
    
    float outputSample = inputSample * gainReductionLinear;
    
    return outputSample;
}

void Compressor::reset()
{
    mEnvelopeFollower.reset();
}

                                                                 
// ==================================================================//  EXPANDER  //==================================================================
Expander::Expander()
{
    mThreshold_dB = -40.0f;
    mRatio = 4.0f;
}

void Expander::prepare(double sampleRate)
{
    mEnvelopeFollower.prepare(sampleRate);
    mEnvelopeFollower.setAttackTime(10.0f);
    mEnvelopeFollower.setReleaseTime(20.0f);
    setThreshold(mThreshold_dB);
    setRatio(mRatio);
}

void Expander::setThreshold (float threshold_dB)
{
    mThreshold_dB = threshold_dB;
}

void Expander::setRatio (float ratio)
{
    mRatio = ratio;
}

void Expander::setAttackTime (float timeMs)
{
    mEnvelopeFollower.setAttackTime(timeMs);
}

void Expander::setReleaseTime (float timeMs)
{
    mEnvelopeFollower.setReleaseTime(timeMs);
}

float Expander::process(float inputSample)
    {
        float envelope = mEnvelopeFollower.processSample(inputSample);
    float envelope_dB = linearTodB(envelope);
        
    float gainReduction_dB = 0.0f;
        
        // KEY DIFFERENCE: Check if BELOW threshold
    if (envelope_dB < mThreshold_dB)
        {
            // Signal is below threshold, apply expansion
            float undershoot_dB = mThreshold_dB - envelope_dB;  // How far below?
            gainReduction_dB = undershoot_dB * (mRatio - 1.0f); // Expand formula
        }
        
    float gainReduction_linear = dBToLinear(-gainReduction_dB);
        
    return inputSample * gainReduction_linear;
    }

void Expander::reset()
{
    mEnvelopeFollower.reset();
}


