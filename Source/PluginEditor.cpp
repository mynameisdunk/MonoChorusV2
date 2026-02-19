/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "LookAndFeel.h"
#include "RotaryKnob.h"

//==============================================================================
MonoChorusV2AudioProcessorEditor::MonoChorusV2AudioProcessorEditor (MonoChorusV2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(rateKnob);
    addAndMakeVisible(depthKnob);
    addAndMakeVisible(mixKnob);

    juce::LookAndFeel::setDefaultLookAndFeel(RotaryKnobLookAndFeel::get());
    
    bypassButton.setLookAndFeel(&pedalButtonLookAndFeel);
    bypassButton.setClickingTogglesState(true);
    bypassButton.setBounds(0, 0, 120, 40);
    addAndMakeVisible(bypassButton);
    bypassButton.onClick = [this]() {repaint();};
    
    setSize (300, 550);
    // ^ editor size MUST BE LAST
}

MonoChorusV2AudioProcessorEditor::~MonoChorusV2AudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    bypassButton.setLookAndFeel(nullptr);
}

//==============================================================================
void MonoChorusV2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
// background
    g.setColour(juce::Colours::darkgrey);
    g.fillAll();
    
// pedal slight border
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(2, 2, 296, 546, 20);
    
// pedal colour
    auto gradient = juce::ColourGradient(juce::Colours::lightblue, 0.0f, getY() + 350, juce::Colours::royalblue, 0.0f, getBottom(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(4, 4, 292, 542, 20);
//    g.setColour(juce::Colours::lightblue);
//    g.fillRoundedRectangle(2, 2, 296, 546, 20);
    
    //==============================================================================
// logo box drop shadow colour
    auto logoOuter = juce::Rectangle<int>(62, 248, 172, 60);
    auto corner = 8.0f;
    juce::Path logoPath;
    logoPath.addRoundedRectangle(logoOuter, corner);
    juce::DropShadow logoShadow(juce::Colours::black, 24, juce::Point<int>(2, 2));
    logoShadow.drawForPath(g, logoPath);
    
// logo box
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(62, 243, 172, 60, corner);
    
// setting up font
    auto f = Fonts::getFont(48.0f);
    auto chorus = juce::String("CHORUS");
    const float pixelWidth = f.getStringWidthFloat(chorus);
    const float pixelHeight = f.getHeight();

// logo box infill
    g.setColour(juce::Colours::lightblue);
    g.fillRoundedRectangle(68, 249, 160, 48, corner);
    //==============================================================================
    
    g.setFont(f);
    g.setColour(juce::Colours::black);
    g.drawText(chorus, 48, 222, 200, 100, juce::Justification::centred);
    
// LED
    auto ledOuter = juce::Rectangle<float>(getWidth()/2.0f - 6.0f, 20.0f, 12.0f, 12.0f);
    juce::Path ledPath;
    ledPath.addEllipse(ledOuter);
    juce::DropShadow ledShadow {juce::Colours::black, 16, {-2, -2} };
    ledShadow.drawForPath(g,ledPath);
  
    g.setColour(juce::Colours::black);
    g.fillEllipse(ledOuter);
    
    bool isOn = bypassButton.getToggleState();
    auto ledArea = juce::Rectangle<float>(getWidth()/2.0f - 6.0f, 20.0f, 12.0f, 12.0f);
    
    g.setColour(isOn ? juce::Colours::red : juce::Colours::darkred);
    g.fillEllipse(ledArea);
    auto highlight = ledOuter.reduced(3);
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    highlight.setX(getWidth() /2.0f - 4.0f);
    highlight.setY(22.0f);
    g.fillEllipse(highlight);
    
    g.setColour(juce::Colours::black);
    g.drawEllipse(ledArea, 2.0f);
    
// Bypass button
    
    auto bypassOuter = juce::Rectangle<int>(84, 394, 128, 68);
    juce::Path bypassPath;
    bypassPath.addRoundedRectangle(bypassOuter, corner);
    juce::DropShadow bypassShadow(juce::Colours::black, 24, juce::Point<int>(-2, -2));
    bypassShadow.drawForPath(g, bypassPath);
    
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(84.0f, 394.0f, 128.0f, 68.0f, 2.0f);
    g.setColour(juce::Colours::darkgrey);
    g.fillRoundedRectangle(87.0f, 397.0f, 122.0f, 62.0f, 2.0f);

// Logo Box
        
    auto logoBox = juce::Rectangle<float>(68.0f, bypassOuter.getBottom() + 22.0f, 160.0f, 40.0f);
    juce::Path logoBoxPath;
    logoBoxPath.addRoundedRectangle(logoBox, corner);
    juce::DropShadow logoBoxShadow(juce::Colours::lightblue, 16, juce::Point<int>(0, 0));
    logoBoxShadow.drawForPath(g, logoBoxPath);
    g.setColour(juce::Colours::lightblue.withAlpha(0.05f));
    g.fillRoundedRectangle(logoBox, 8.0f);
        
// Logo
    g.setColour(juce::Colours::lightblue.withAlpha(0.95f));
    auto logo = juce::ImageCache::getFromMemory(BinaryData::Logo_V1_png, BinaryData::Logo_V1_pngSize);
    int logoWidth = logo.getWidth();
    int logoHeight = logo.getHeight();
    g.drawImage(logo, 50, 406, (logoWidth / 5.0f), (logoHeight / 5.0f), 0, 0, logo.getWidth(), logo.getHeight());
    
}

void MonoChorusV2AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    rateKnob.setBounds(32, 35, 100, 116);
    depthKnob.setBounds(rateKnob.getRight() + 32, 35, 100, 116);
    mixKnob.setBounds(108, 126, 80, 96);
    bypassButton.setBounds(88, 398, 120, 60);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    paramGroup.setBounds(0, 0, 280, 150);
}





//THIS IS APPLYING A TEXTURE TO THE IMAGE. DOESN'T LOOK SO GOOD :(

/*     auto noise = juce::ImageCache::getFromMemory(BinaryData::crackled_paint_jpg, BinaryData::crackled_paint_jpgSize);
 auto fillType = juce::FillType(noise, juce::AffineTransform::scale(0.0f));
 
    // Scale the texture to your editor size
    juce::Rectangle<float> dst = getLocalBounds().toFloat();

    // Use a Graphics::ScopedSaveState if you plan to tweak blending
    juce::Graphics::ScopedSaveState save(g);

    // Set multiplicative blending via composite rule
    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
    g.setOpacity(0.15f); // control strength of the patina
auto imgH = noise.getHeight();
auto imgW = noise.getWidth();
// Bottom-left quarter: x = 0, y = imgH/2, width = imgW/2, height = imgH/2
juce::Rectangle<int> src(0, imgH / 2, imgW / 2, imgH / 2);

// Draw only that sub-rectangle, scaled to fill the destination
g.drawImage(noise,
            dst.getX(), dst.getY(), dst.getWidth(), dst.getHeight(), // destination rect
            src.getX(), src.getY(), src.getWidth(), src.getHeight()); // source rect
*/
