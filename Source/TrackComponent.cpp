/*
  ==============================================================================

    TrackComponent.cpp
    Created: 15 Jul 2015 8:31:56am
    Author:  R00070693

  ==============================================================================
*/

#include "TrackComponent.h"

TrackComponent::TrackComponent(AudioFormatManager& formatMan, AudioTransportSource& transportSource_) : isMuted(false), followsTransport(false), transportSource(transportSource_)
{
	addAndMakeVisible(zoomSlider = new Slider());
	addAndMakeVisible(thumbnail = new ThumbnailComp(formatMan, transportSource, *zoomSlider));
	thumbnail->addChangeListener(this);
	addAndMakeVisible(gainSlider = new Slider("GainSlider"));
	gainSlider->setSliderStyle(Slider::SliderStyle::LinearBarVertical);

	addAndMakeVisible(muteButton = new TextButton("M"));
	muteButton->addListener(this);
	
	zoomSlider->setSliderStyle(Slider::SliderStyle::LinearVertical);
	zoomSlider->setRange(0, 1, 0);
	zoomSlider->setSkewFactor(2);
	zoomSlider->addListener(this);
	addAndMakeVisible(trackNameLabel = new Label("File"));

	setSize(getParentWidth(), 100);
}

TrackComponent::~TrackComponent()
{
	zoomSlider = nullptr;
	thumbnail = nullptr;
	gainSlider = nullptr;
	muteButton = nullptr;
	trackNameLabel = nullptr;
}

void TrackComponent::paint(Graphics& g)
{
	g.fillAll(Colours::black);

	Rectangle<int> trackArea = getLocalBounds();
	g.setColour(Colours::burlywood);
	g.fillRect(trackArea);
	g.setColour(Colours::black);
	g.drawRect(trackArea);

	Rectangle<int> trackControls = trackArea.removeFromLeft(100);
	g.setColour(Colours::lightslategrey);
	g.fillRect(trackControls);
	g.setColour(Colours::black);
	g.drawRect(trackControls);

	thumbnail->setBounds(trackArea);
	trackNameLabel->setBounds(trackControls.removeFromTop(20));

	Rectangle <int> trackInfo = trackControls.removeFromLeft(trackControls.getWidth()/2);
	gainSlider->setBounds(trackInfo.removeFromLeft(20));
	zoomSlider->setBounds(trackInfo.removeFromLeft(20));
	muteButton->setBounds(trackControls.removeFromBottom(30).reduced(5));

}

void TrackComponent::resized()
{
	
}

void TrackComponent::setFile(File& f)
{
	thumbnail->setFile(f);
	fileName = f.getFileName();
	trackNameLabel->setSize(60, 20);
	trackNameLabel->setText(fileName, dontSendNotification);
	trackNameLabel->setTooltip(fileName);
}

void TrackComponent::sliderValueChanged(Slider* sliderThatWasChanged)
{
	if (zoomSlider == sliderThatWasChanged)
	{
		thumbnail->setZoomFactor(zoomSlider->getValue());
	}
	else if (gainSlider == sliderThatWasChanged)
	{
		gain = gainSlider->getValue();
	}
}

void TrackComponent::changeListenerCallback(ChangeBroadcaster* broadcastSource)
{
	if (thumbnail == broadcastSource)
	{
		thumbnail->repaint();
	}
}

void TrackComponent::buttonClicked(Button* buttonThatWasClicked)
{
	if (muteButton == buttonThatWasClicked)
	{
		if (!isMuted)
		{
			// mute track
			isMuted = true;
			muteButton->setColour(TextButton::buttonColourId, Colours::indianred);		
		}
		else
		{
			isMuted = false;
			muteButton->setColour(TextButton::buttonColourId, Colour(187, 187, 255));
		}
	}
}

void TrackComponent::setFollowsTransport(bool shouldFollow)
{
	thumbnail->setFollowsTransport(shouldFollow);
}

