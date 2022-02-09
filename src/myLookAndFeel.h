/*
  ==============================================================================

    Created: 14 Dec 2017 10:16:04am
    Author:  Stefan Remberg

    Modified by keyth72

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
class myLookAndFeel : public LookAndFeel_V4
{

public:
    myLookAndFeel();
    void setLookAndFeel(Image inputImage);
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

private:
    Image img;

};
