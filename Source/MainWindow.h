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

#ifndef __JUCE_HEADER_1A7C0314FA0C68BA__
#define __JUCE_HEADER_1A7C0314FA0C68BA__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PanExtractor.h"
#include "Eigen\Eigen"
#include "STFT.h"
#include "ISTFT.h"
#include "Constants.h"
#include "About.h"
#include "LoadTask.h"
#include "TrackComponent.h"
#include "PositionableMixerSource.h"
#include "ToolbarFactory.h"

using Eigen::MatrixXcf;
using juce::AudioSampleBuffer;
using juce::Array;

enum direction
{
	LEFT = 1,
	CENTRE = 2,
	RIGHT = 3
};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainWindow  : public AudioAppComponent,
                    public MenuBarModel,
                    public ApplicationCommandTarget,
                    public KeyListener,
                    public ChangeListener,
                    public ButtonListener,
                    public ComboBoxListener
{
public:
    //==============================================================================
    MainWindow ();
    ~MainWindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	bool isInterestedInFileDrag(const StringArray& /*files*/);
	bool loadFile(File&);
	void inverseTransform(int direction);
	void write(int direction);

	// implementation of MenuBarModel methods ======================================
	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
	// =============================================================================

	// implementation of ApplicationCommandTarget methods ==========================
	ApplicationCommandTarget* getNextCommandTarget() override;
	void getAllCommands(juce::Array<CommandID> &commands) override;
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;
	bool perform(const InvocationInfo& info) override;
	// =============================================================================


	void closeButtonPressed();

	// implementation of AudioAppComponent methods =================================
	void prepareToPlay(int, double) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo & channelInfo) override;
	void releaseResources() override;
	// =============================================================================

	void changeListenerCallback(ChangeBroadcaster * broadcastSource) override;
	bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	CriticalSection lock;
	double currentSampleRate;
	int currentBlockSize;

	int noOfCols;
	int panDirection;
	String fileName;
	bool fileLoaded, following;
	ScopedPointer<File> outFile;
	ScopedPointer<PanExtractor> extractor;

	// matrices to hold transformed results (spectral data)
	MatrixXcf frequencyMatrix_LeftChannel;
	MatrixXcf frequencyMatrix_RightChannel;

	// STFT objects to perform transform, ISTFT for inverse
	STFT stft_L;
	STFT stft_R;
	ISTFT istft_L;
	ISTFT istft_R;

	// pointers to hold data from sampleBuffer
	float* bufferData_L;
	float* bufferData_R;
	// pointers to results arrays
	float* fftResults_L;
	float* fftResults_R;
	float ifftResults_L[WINDOW_SIZE];
	float ifftResults_R[WINDOW_SIZE];

	std::complex<float>* comp_L;
	std::complex<float>* comp_R;

	// Arrays to hold actual resynthesized output
	juce::Array<float> signal_left;
	juce::Array<float> signal_right;

	// Audio =======================
	bool playing;
	OwnedArray<AudioFormatReaderSource, CriticalSection> readerSources;
	ScopedPointer<AudioSampleBuffer> outSamples;
	ScopedPointer<WavAudioFormat> wav;
	PositionableMixerSource mixerSource;
	AudioFormatManager formatManager;
	AudioTransportSource transportSource;
	AudioSourcePlayer sourcePlayer;
	// =============================

	// GUI =========================
	ScopedPointer<Label> itersLabel;
	Toolbar toolbar;
	ToolbarFactory toolbarFactory;
	ScopedPointer<MenuBarComponent> menuBar;
	ApplicationCommandManager commandManager;
	ScopedPointer<FileChooser> chooser;
	ScopedPointer<About> aboutComp;
	ScopedPointer<TrackComponent> leftTrack, centreTrack, rightTrack;
	ScopedPointer<AudioDeviceSelectorComponent> settingsComp;
	// =============================

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> helpLabel;
    ScopedPointer<TextButton> extractButton;
    ScopedPointer<ComboBox> iterations;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_1A7C0314FA0C68BA__
