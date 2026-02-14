#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "RotaryKnob.h"

const juce::Typeface::Ptr Fonts::typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::GoogleSansCodeBold_ttf, BinaryData::GoogleSansCodeBold_ttfSize);

juce::Font Fonts::getFont(float height)
{
    return juce::FontOptions(typeface) .withMetricsKind(juce::TypefaceMetricsKind::legacy)
                        .withHeight(height);
}



//=================================================================================================================



juce::Font RotaryKnobLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label)
{
    return Fonts::getFont();
}

//=================================================================================================================

RotaryKnobLookAndFeel::RotaryKnobLookAndFeel()
{
    setColour(juce::Label::textColourId, Colours::Knob::label);
    setColour(juce::Slider::textBoxTextColourId, Colours::Knob::label);
    setColour(juce::Slider::rotarySliderFillColourId, Colours::Knob::echoTrackActive);
    setColour(juce::Slider::textBoxTextColourId, Colours::Knob::label);
    setColour(juce::CaretComponent::caretColourId, Colours::Knob::caret);
    
}

//=================================================================================================================

void RotaryKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, [[maybe_unused]] int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, width).toFloat();
    auto knobRect = bounds.reduced(10.0f, 10.0f);
    
    auto path = juce::Path();
    path.addEllipse(knobRect);
    dropShadow.drawForPath(g,path);
  
    g.setColour(juce::Colours::black);
    g.fillEllipse(knobRect);
    // ^^ this is the main circle knob seen
    
    // TRYING TO ADD ANOTHER CIRCLE (I think I did it!!!!)
    
    auto knobOutline = bounds.reduced(12.0f, 12.0f);
    auto pathOutline = juce::Path();
    path.addEllipse(knobOutline);
    
    g.setColour(juce::Colours::grey);
    g.fillEllipse(knobOutline);
    
    
    auto innerRect = knobRect.reduced(6.0f, 6.0f);
    //auto gradient = juce::ColourGradient(juce::Colours::darkgrey, 0.0f, innerRect.getY(), juce::Colours::lightgrey, 0.0f, innerRect.getBottom(), false);
    //g.setGradientFill(gradient);
    g.setColour(juce::Colours::black);
    g.fillEllipse(innerRect);
    // ^^ this block is creating a second smaller circle inside the main which gives the impression of a 3d knob
    
   // ============================================================================================================================================
    
    auto center = bounds.getCentre();
    auto radius = bounds.getWidth() / 2.0f;
    auto lineWidth = 3.0f;
    auto arcRadius = radius - lineWidth/2.0f;
    
    
    auto strokeType = juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    
    /*
    // Background track (inactive ring)
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(Colours::Knob::trackBackground);
    g.strokePath(backgroundArc, strokeType);
    */
    
    // ^^ this block is creating a track around the knob
    
    
    auto dialRadius = innerRect.getHeight() / 2.0f - lineWidth;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    juce::Point<float> dialStart(center.x + 10.0f * std::sin(toAngle),
                                 center.y - 10.0f * std::cos(toAngle));
                                 
    juce::Point<float> dialEnd(center.x + dialRadius * std::sin(toAngle),
                               center.y - dialRadius * std::cos(toAngle));
    
    juce::Path dialPath;
    dialPath.startNewSubPath(dialStart);
    dialPath.lineTo(dialEnd);
    g.setColour(juce::Colours::red);
    g.strokePath(dialPath, strokeType);
    
    // ============================================================================================================================================
    
    /*
    // Active value arc (follows the knob value)
    if (slider.isEnabled())
    {
        float fromAngle = rotaryStartAngle;
        if (slider.getProperties()["drawFromMiddle"]) {
            fromAngle += (rotaryEndAngle - rotaryStartAngle) / 2.0f;
        }
        juce::Path valueArc;
        valueArc.addCentredArc(center.x, center.y, arcRadius, arcRadius, 0.0f, fromAngle, toAngle, true);
        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        g.strokePath(valueArc, strokeType);
    }
    */
}

//=================================================================================================================

class RotaryKnobLabel : public juce::Label
{
public:
        RotaryKnobLabel() : juce::Label()
        {}
        void mouseWheelMove(const juce::MouseEvent&,
                            const juce::MouseWheelDetails&) override
        {}
    
    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override
    {
    return createIgnoredAccessibilityHandler(*this);
    }
    
    juce::TextEditor* createEditorComponent() override {
    auto* ed = new juce::TextEditor(getName()); ed->applyFontToAllText(getLookAndFeel().getLabelFont(*this)); copyAllExplicitColoursTo(*ed);
        ed->setBorder(juce::BorderSize<int>());
        ed->setIndents(2, 1);
        ed->setJustification(juce::Justification::centredTop);
        
        ed->setPopupMenuEnabled(false);
        ed->setInputRestrictions(8);
        
        return ed;
    }
};

//=================================================================================================================

juce::Label* RotaryKnobLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    auto l = new RotaryKnobLabel();
        l->setJustificationType(juce::Justification::centred);
        l->setKeyboardType(juce::TextInputTarget::decimalKeyboard);
        l->setColour(juce::Label::textColourId,
                 slider.findColour(juce::Slider::textBoxTextColourId));
        l->setColour(juce::TextEditor::textColourId, Colours::Knob::value);
        l->setColour(juce::TextEditor::highlightedTextColourId, Colours::Knob::value);
        l->setColour(juce::TextEditor::highlightColourId,
                 slider.findColour(juce::Slider::rotarySliderFillColourId));
        l->setColour(juce::TextEditor::backgroundColourId,
                 Colours::Knob::textBoxBackground);
    return l;
    
}

//=====================================R============================================================================
/*
MainLookAndFeel::MainLookAndFeel()
{
    setColour(juce::GroupComponent::textColourId, Colours::Group::label);
    setColour(juce::GroupComponent::outlineColourId, Colours::echoHeader);
    
}
*/
//=================================================================================================================

juce::Font MainLookAndFeel::getLabelFont([[maybe_unused]] juce::Label& label)
{
    return Fonts::getFont();
}

juce::Font MainLookAndFeel::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    return Fonts::getFont(juce::jlimit(12.0f, 20.0f, (float) buttonHeight * 0.5f));
}

juce::Font MainLookAndFeel::getPopupMenuFont()
{
    return Fonts::getFont(14.0f);
}

juce::Font MainLookAndFeel::getComboBoxFont(juce::ComboBox&)
{
    return Fonts::getFont(14.0f);
}

juce::Font MainLookAndFeel::getAlertWindowTitleFont()
{
    return Fonts::getFont(18.0f);
}

juce::Font MainLookAndFeel::getAlertWindowMessageFont()
{
    return Fonts::getFont(14.0f);
}

//=================================================================================================================

void RotaryKnobLookAndFeel::fillTextEditorBackground(
juce::Graphics& g, [[maybe_unused]] int width, [[maybe_unused]] int height, juce::TextEditor& textEditor)
{
    g.setColour(Colours::Knob::textBoxBackground);
    g.fillRoundedRectangle(textEditor.getLocalBounds().reduced(4, 0).toFloat(), 4.0f);
}
/*
ButtonLookAndFeel::ButtonLookAndFeel()
{
    setColour(juce::TextButton::textColourOffId, Colours::Button::text);
    setColour(juce::TextButton::textColourOnId, Colours::Button::textToggled);
    setColour(juce::TextButton::buttonColourId, Colours::Button::background);
    setColour(juce::TextButton::buttonOnColourId,
    Colours::Button::backgroundToggled);
}

void ButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, [[maybe_unused]] bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto cornerSize = bounds.getHeight() * 0.25f;
        auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);
        if (shouldDrawButtonAsDown)
        {
        buttonRect.translate(0.0f, 1.0f);
        }
        g.setColour(backgroundColour);
        g.fillRoundedRectangle(buttonRect, cornerSize);
        g.setColour(Colours::Button::outline);
        g.drawRoundedRectangle(buttonRect, cornerSize, 2.0f);
    }

void ButtonLookAndFeel::drawButtonText(
    juce::Graphics& g, juce::TextButton& button, [[maybe_unused]] bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto buttonRect = bounds.reduced(1.0f, 1.0f).withTrimmedBottom(1.0f);
        if (shouldDrawButtonAsDown)
        {
        buttonRect.translate(0.0f, 1.0f);
        }
        if (button.getToggleState()) {
        g.setColour(button.findColour(juce::TextButton::textColourOnId));
        } else {
        g.setColour(button.findColour(juce::TextButton::textColourOffId));
        }
        g.setFont(Fonts::getFont());
        g.drawText(button.getButtonText(), buttonRect, juce::Justification::centred);
    }

*/

