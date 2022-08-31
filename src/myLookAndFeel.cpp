/*
  ==============================================================================

    Created: 14 Dec 2017 10:16:04am
    Author:  Stefan Remberg

    Modified by keyth72

  ==============================================================================
*/

#include "myLookAndFeel.h"

//==============================================================================
myLookAndFeel::myLookAndFeel()
{
}

//==============================================================================
void myLookAndFeel::setLookAndFeel(Image inputImage)
{
    // Edit this line to match png file from project Resources
    img = inputImage;
}


//==============================================================================
void myLookAndFeel::drawRotarySlider(Graphics& g,
    int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    const double rotation = (slider.getValue()
        - slider.getMinimum())
        / (slider.getMaximum()
            - slider.getMinimum());

    const int frames = img.getHeight() / img.getWidth();
    const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
    const float radius = jmin(width / 2.0f, height / 2.0f);
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float rx = centerX - radius - 1.0f;
    const float ry = centerY - radius;

    g.drawImage(img,
        (int)rx,
        (int)ry,
        2 * (int)radius,
        2 * (int)radius,
        0,
        frameId*img.getWidth(),
        img.getWidth(),
        img.getWidth());
}
