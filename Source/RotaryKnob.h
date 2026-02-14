
#pragma once

#include <JuceHeader.h>

class RotaryKnob  : public juce::Component
{
public:
    RotaryKnob(const juce::String& text,
               juce::AudioProcessorValueTreeState& apvts,
               const juce::ParameterID& paramID,
               bool drawFromMiddle = false,
               bool showTextBox = true);
    
    ~RotaryKnob() override;

    void resized() override;
    
    juce::Slider slider;  //    -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -     // This is what it attaches to
    juce::Label label;
    
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;            // This is an attachment (duh) but ^^
    
    // Any attachment must be declared AFTER the component it attaches to

private:
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryKnob)
};
