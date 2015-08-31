/*
  ==============================================================================

    TrackComponent.h
    Created: 15 Jul 2015 8:31:56am
    Author:  R00070693

	Component that contains track controls and audio thumbnail display

  ==============================================================================
*/

#ifndef TRACKCOMPONENT_H_INCLUDED
#define TRACKCOMPONENT_H_INCLUDED
#include "..\JuceLibraryCode\JuceHeader.h"
#include "ThumbnailComp.h"

class TrackComponent : public Component,
	public Slider::Listener,
	public Button::Listener,
	public ChangeListener,
	public ChangeBroadcaster
{
public:

	/*
		Constructor
		input: AudioFormatManager to handle reading wav, AudioTransportSource to display cursor and enable playback
	*/
	TrackComponent(AudioFormatManager&, AudioTransportSource&);

	~TrackComponent();

	/*
		implementation of Component method
	*/
	void paint(Graphics& g) override;

	/*
		implementation of Component method
	*/
	void resized() override;

	/*
		sets file that track will control
		input: file to control
		output: none
	*/
	void setFile(File& f);

	/*
		implementation of Slider::Listener method
	*/
	void sliderValueChanged(Slider*) override;

	/*
		implemetation of ChangeListener method
	*/
	void changeListenerCallback(ChangeBroadcaster*) override;

	/*
		implementation of Button::Listener method
	*/
	void buttonClicked(Button*) override;

	/*
		sets if component should keep focus on transport cursor
	*/
	void setFollowsTransport(bool follow);

private:

	Colour muteOnColour;
	Colour muteOffColour;
	AudioTransportSource& transportSource;

	ScopedPointer<ThumbnailComp> thumbnail;
	ScopedPointer<TextButton> muteButton;
	ScopedPointer<Slider> volumeSlider, gainSlider, zoomSlider;
	ScopedPointer<Label> trackNumberLabel, trackNameLabel, gainLabel, zoomLabel;
	ScopedPointer<AudioFormatManager> formatManager;

	String fileName;
	bool isMuted, followsTransport;
	double gain, lastGain;

};



#endif  // TRACKCOMPONENT_H_INCLUDED
