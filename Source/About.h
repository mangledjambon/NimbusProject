/*
  ==============================================================================

    About.h
    Created: 3 Jul 2015 9:26:23am
    Author:  R00070693

  ==============================================================================
*/

#ifndef ABOUT_H_INCLUDED
#define ABOUT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class About    : public Component
{
public:
    About()
    {}

    ~About()
    {}

    void paint (Graphics& g)
    {

        g.fillAll (Colours::lightgrey);   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::black);
        g.setFont (14.0f);
        g.drawFittedText (
			JUCEApplication::getInstance()->getApplicationName()
			+ "\nv" + 
			JUCEApplication::getInstance()->getApplicationVersion()
			+ "\nAuthors: Sean Breen, Derry Fitzgerald", getLocalBounds(),
                    Justification::centred, true); 
    }

    void resized()
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (About)
};


#endif  // ABOUT_H_INCLUDED
