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

#ifndef __JUCE_HEADER_464CC7A07742E07A__
#define __JUCE_HEADER_464CC7A07742E07A__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PositionableMixerSource.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    Class to display waveform of audio file
                                                                    //[/Comments]
*/
class ThumbnailComp  : public Component,
                       public ChangeBroadcaster,
                       public ChangeListener,
                       private Timer,
                       public ScrollBar::Listener
{
public:
    //==============================================================================
    ThumbnailComp (AudioFormatManager& formatManager, AudioTransportSource& transportSource_, Slider& slider);
    ~ThumbnailComp();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	/*
		implementation of ChangeListener method
	*/
	void changeListenerCallback(ChangeBroadcaster* source) override;

	/*
		sets viewable range on audio thumbnail
		input: new range as double
		output: none
	*/
	void setRange(Range<double> newRange);

	/*
		set if thumbnail should follow cursor or not
		input: bool 
		output: none
	*/
	void setFollowsTransport(bool shouldFollow);

	/*
		sets the file that the audiothumbnail should display
		input: file to display
		output: none
	*/
	void setFile(const File& file);

	/*
		sets the overall zoom factor for the thumbnail
		input: amount to zoom
		output: none
	*/
	void setZoomFactor(double amount);

	/*
		mouse handler methods
	*/
	void mouseDown(const MouseEvent& e) override;
	void mouseDrag(const MouseEvent& e) override;
	void mouseUp(const MouseEvent& e) override;
	void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	AudioTransportSource& transportSource;
	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;
	File lastFileDropped;
	ScrollBar scrollbar;
	Slider& zoomSlider;
	Range<double> visibleRange;
	bool isFollowingTransport;
	DrawableRectangle currentPositionMarker;

	float timeToX(const double time) const
	{
		return getWidth() * (float)((time - visibleRange.getStart()) / (visibleRange.getLength()));
	}

	double xToTime(const float x) const
	{
		return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
	}

	bool canMoveTransport() const noexcept
	{
		return !(isFollowingTransport && transportSource.isPlaying());
	}

		void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override
	{
		if (scrollBarThatHasMoved == &scrollbar)
			if (!(isFollowingTransport && transportSource.isPlaying()))
				setRange(visibleRange.movedToStartAt(newRangeStart));
	}

	void timerCallback() override
	{
		if (canMoveTransport())
			updateCursorPosition();
		else
			setRange(visibleRange.movedToStartAt(transportSource.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
	}

	void updateCursorPosition()
	{
		currentPositionMarker.setVisible(transportSource.isPlaying() || isMouseButtonDown());

		currentPositionMarker.setRectangle(Rectangle<float>(timeToX(transportSource.getCurrentPosition()) - 0.75f, 0,
			1.5f, (float)(getHeight() - scrollbar.getHeight())));
	}
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailComp)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_464CC7A07742E07A__
