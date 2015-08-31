/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ThumbnailComp.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ThumbnailComp::ThumbnailComp (AudioFormatManager& formatManager, AudioTransportSource& transportSource_, Slider& slider)
    : transportSource(transportSource_),      zoomSlider (slider),
      scrollbar (false),      thumbnailCache (5), thumbnail (512, formatManager, thumbnailCache),       isFollowingTransport (false)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	thumbnail.addChangeListener(this);

	addAndMakeVisible(scrollbar);
	scrollbar.setRangeLimits(visibleRange);
	scrollbar.setAutoHide(false);
	scrollbar.addListener(this);

	currentPositionMarker.setFill(Colours::white.withAlpha(0.85f));
	addAndMakeVisible(currentPositionMarker);
    //[/Constructor]
}

ThumbnailComp::~ThumbnailComp()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
	scrollbar.removeListener(this);
	thumbnail.removeChangeListener(this);
    //[/Destructor]
}

//==============================================================================
void ThumbnailComp::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(Colours::darkgrey);
	g.setColour(Colours::limegreen);

	if (thumbnail.getTotalLength() > 0.0)
	{
		Rectangle<int> thumbArea(getLocalBounds());
		thumbArea.removeFromBottom(scrollbar.getHeight() + 4);
		thumbnail.drawChannels(g, thumbArea.reduced(2),
			visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
	}
	else
	{
		g.setFont(14.0f);
		g.drawFittedText("(No audio file selected)", getLocalBounds(), Justification::centred, 2);
	}
    //[/UserPaint]
}

void ThumbnailComp::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
	scrollbar.setBounds(getLocalBounds().removeFromBottom(14).reduced(2));
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ThumbnailComp::changeListenerCallback(ChangeBroadcaster* source)
{
	repaint();
}

void ThumbnailComp::setRange(Range<double> newRange)
{
	visibleRange = newRange;
	scrollbar.setCurrentRange (visibleRange);
	updateCursorPosition();
	repaint();
}

void ThumbnailComp::setFollowsTransport(bool shouldFollow)
{
	isFollowingTransport = shouldFollow;
}

void ThumbnailComp::setFile(const File& file)
{
	if (!file.isDirectory())
	{
		thumbnail.setSource (new FileInputSource (file));
		const Range<double> newRange (0.0, thumbnail.getTotalLength());
		scrollbar.setRangeLimits (newRange);
		setRange (newRange);
		sendChangeMessage();
		startTimerHz (60);
	}
}

void ThumbnailComp::setZoomFactor(double amount)
{
	if (thumbnail.getTotalLength() > 0)
	{
		const double newScale = jmax(0.001, thumbnail.getTotalLength() * (1.0 - jlimit(0.0, 0.99, amount)));
		const double timeAtCentre = xToTime (getWidth() / 2.0f);
		setRange (Range<double> (timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));
	}
}

void ThumbnailComp::mouseDown(const MouseEvent& e)
{
	mouseDrag(e);
}

void ThumbnailComp::mouseDrag(const MouseEvent& e)
{
	if (canMoveTransport())
		transportSource.setPosition(jmax(0.0, xToTime((float)e.x)));
}

void ThumbnailComp::mouseUp(const MouseEvent&)
{
	transportSource.start();
}

void ThumbnailComp::mouseWheelMove(const MouseEvent&, const MouseWheelDetails& wheel)
{
	if (thumbnail.getTotalLength() > 0.0)
	{
		double newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
		newStart = jlimit(0.0, jmax(0.0, thumbnail.getTotalLength() - (visibleRange.getLength())), newStart);

		if (canMoveTransport())
			setRange(Range<double>(newStart, newStart + visibleRange.getLength()));

		if (wheel.deltaY != 0.0f)
			zoomSlider.setValue(zoomSlider.getValue() - wheel.deltaY);

		repaint();
	}
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ThumbnailComp" componentName=""
                 parentClasses="public Component, public ChangeBroadcaster, public ChangeListener, private Timer, public ScrollBar::Listener"
                 constructorParams="AudioFormatManager&amp; formatManager, AudioTransportSource&amp; transportSource_, Slider&amp; slider"
                 variableInitialisers="transportSource(transportSource_),      zoomSlider (slider),&#10;scrollbar (false),      thumbnailCache (5), thumbnail (512, formatManager, thumbnailCache),       isFollowingTransport (false)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
