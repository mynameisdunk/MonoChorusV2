#pragma once
#include <JuceHeader.h>
//==============================================================================================================
namespace Colours
{
    const juce::Colour background { 245, 240, 235 };
    const juce::Colour header { 40, 40, 40 };
    const juce::Colour echoGreen { 125, 141, 102 };
    const juce::Colour echoHeader { 50, 49, 46 };

    namespace Knob
    {
        const juce::Colour trackBackground { 205, 200, 195 };
        const juce::Colour trackActive { 177, 101, 135 };
        const juce::Colour echoTrackActive { 134, 57, 51 };
        const juce::Colour outline { 255, 250, 245 };
        const juce::Colour gradientTop { 250, 245, 240 };
        const juce::Colour gradientBottom { 240, 235, 230 };
        const juce::Colour dial { 100, 100, 100 };
        const juce::Colour dropShadow { 195, 190, 185 };
        const juce::Colour label { 80, 80, 80 };
        const juce::Colour textBoxBackground { 80, 80, 80 };
        const juce::Colour value { 240, 240, 240 };
        const juce::Colour caret { 255, 255, 255 };
    }

    namespace Group
    {
        const juce::Colour label {160, 155, 150};
        const juce::Colour outline {235, 230 , 225};
    }

    namespace Button
    {
        const juce::Colour text { 80, 80, 80 };
        const juce::Colour textToggled { 40, 40, 40 };
        const juce::Colour background { 245, 240, 235 };
        const juce::Colour backgroundToggled { 255, 250, 245 };
        const juce::Colour outline { 235, 230, 225 };
    }

    namespace LevelMeter
    {
    const juce::Colour background { 245, 240, 235 };
    const juce::Colour tickLine { 200, 200, 200 };
    const juce::Colour tickLabel { 80, 80, 80 };
    const juce::Colour tooLoud { 226, 74, 81 };
    const juce::Colour levelOK { 65, 206, 88 };
    }
}

//==============================================================================================================

class Fonts
{
public:
    
    Fonts () = delete;
    static juce::Font getFont(float height = 16.0f);
    
private:
    
    static const juce::Typeface::Ptr typeface;
    
};

//==============================================================================================================


class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RotaryKnobLookAndFeel();
    
    static RotaryKnobLookAndFeel* get()
    {
        static RotaryKnobLookAndFeel instance;
        return &instance;
    }
    
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override;
    
    juce::Font getLabelFont(juce::Label&) override;
    
    juce::Label* createSliderTextBox(juce::Slider&) override;
    
    void drawTextEditorOutline(juce::Graphics&, int, int, juce::TextEditor&) override {}
    
    void fillTextEditorBackground(juce::Graphics&, int width, int height, juce::TextEditor&) override;
    
private:
    
    juce::DropShadow dropShadow {juce::Colours::black, 9, {-4, -3} };
                            // colour                , radius, offset (x, y)
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)
    
    
};

class MainLookAndFeel : public juce::LookAndFeel_V4 {
public:
    MainLookAndFeel();
    
    juce::Font getLabelFont(juce::Label&) override;

    juce::Font getTextButtonFont(juce::TextButton& button, int buttonHeight) override;
    juce::Font getPopupMenuFont() override;
    juce::Font getComboBoxFont(juce::ComboBox& box) override;
    juce::Font getAlertWindowTitleFont() override;
    juce::Font getAlertWindowMessageFont() override;
    
private:
        
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};

class PedalButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    
    PedalButtonLookAndFeel() = default;
    
    void drawButtonBackground (juce::Graphics& g,
                               juce::Button& button,
                               const juce::Colour&,
                               bool,
                               bool) override
    {
        
        auto bounds = button.getLocalBounds().toFloat();
        auto highlightX = bounds;
        auto highlightY = bounds;
        auto highlightTopX = bounds;
        auto highlightTopY = bounds;

        bool isOn = button.getToggleState();

        float pressOffsetX = isOn ? 1.0f : 0.0f;
        float pressOffsetY = isOn ? 1.0f : 0.0f;
        g.addTransform(juce::AffineTransform::translation(pressOffsetX, pressOffsetY));

        g.setColour (juce::Colours::black);
        g.fillRoundedRectangle (bounds, 2.0f);
// Lower Highlight
        g.setColour(juce::Colours::white.withAlpha(0.2f));
        highlightX.setBounds(13.0f, 57.0f, 106.0f, 2.0f);
        g.fillRoundedRectangle(highlightX, 0.0f);
        highlightY.setBounds(117.0f, 15.0f, 2.0f, 42.0f);
        g.fillRoundedRectangle(highlightY, 0.0f);
        
// Top Highlight
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        highlightTopX.setBounds(3.0f, 3.0f, 106.0f, 2.0f);
        g.fillRoundedRectangle(highlightTopX, 0.0f);
        highlightTopY.setBounds(3.0f, 5.0f, 2.0f, 42.0f);
        g.fillRoundedRectangle(highlightTopY, 0.0f);
        
        
        
//        g.fillRoundedRectangle(12.0f,56.0f, 106.0f, 2.0f, 0.0f);
//        g.fillRoundedRectangle(116.0f, 14.0f, 2.0f, 42.0f, 0.0f);
        
        // bypassButton.setBounds(88, 440, 120, 60); // keep for reference for graphics above
        
    }
    
    private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PedalButtonLookAndFeel)
};
