/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"
//==============================================================================
/**
*/
class MonoChorusV2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MonoChorusV2AudioProcessorEditor (MonoChorusV2AudioProcessor&);
    ~MonoChorusV2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MonoChorusV2AudioProcessor& audioProcessor;

    RotaryKnob mixKnob {"MIX", audioProcessor.apvts, mixParamID, false, false};
    RotaryKnob rateKnob {"RATE", audioProcessor.apvts, rateParamID, false, false};
    RotaryKnob depthKnob {"DEPTH", audioProcessor.apvts, depthParamID, false, false};
    
    juce::GroupComponent paramGroup;
    
    juce::DropShadow edgeBlur {juce::Colours::black, 6, {0, 0}};
    
    juce::TextButton bypassButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment {audioProcessor.apvts, bypassParamID.getParamID(), bypassButton};
    
    PedalButtonLookAndFeel pedalButtonLookAndFeel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonoChorusV2AudioProcessorEditor)
};
