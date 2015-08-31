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
/*
	TODO	fix mixer/transport cursor placement
	TODO	get mixer gain control working
	TODO	get track gain control working
*/
//[/Headers]

#include "MainWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

enum CommandIDs // enumerated type to simplify command ids
{
	load = 0x2001,
	save = 0x2002,
	settings = 0x2003,
	about = 0x2004,
	play = 0x2005,
	followTransport = 0x2006,
	pause = 0x2007,
	stop = 0x2008,
	quit = StandardApplicationCommandIDs::quit

};

//[/MiscUserDefs]

//==============================================================================
MainWindow::MainWindow ()
    : extractor(new PanExtractor()), panDirection(LEFT), stft_L(WINDOW_SIZE), stft_R(WINDOW_SIZE), playing(false)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("mainWindow");
    addAndMakeVisible (helpLabel = new Label ("help",
                                              TRANS("Drag an audio file to load...")));
    helpLabel->setFont (Font (15.00f, Font::plain));
    helpLabel->setJustificationType (Justification::centredLeft);
    helpLabel->setEditable (false, false, false);
    helpLabel->setColour (TextEditor::textColourId, Colours::black);
    helpLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (extractButton = new TextButton ("extract"));
    extractButton->setTooltip (TRANS("Extract sources from audio file."));
    extractButton->setButtonText (TRANS("Extract Sources"));
    extractButton->addListener (this);
    extractButton->setColour (TextButton::buttonColourId, Colour (0xffcdffbb));

    addAndMakeVisible (iterations = new ComboBox ("iterations"));
    iterations->setEditableText (true);
    iterations->setJustificationType (Justification::centredLeft);
    iterations->setTextWhenNothingSelected (TRANS("No. of iterations"));
    iterations->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    iterations->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (300, 300);


    //[Constructor] You can add your own custom stuff here..

	toolbar.addDefaultItems(toolbarFactory);
	addAndMakeVisible(toolbar);

	addAndMakeVisible(menuBar = new MenuBarComponent(this));
	menuBar->setEnabled(true);

	iterations->addItem(String("1"), 1);
	iterations->addItem(String("10"), 2);
	iterations->addItem(String("100"), 3);
	iterations->setSelectedId(1, dontSendNotification);

	addAndMakeVisible(leftTrack = new TrackComponent(formatManager, transportSource));
	addAndMakeVisible(centreTrack = new TrackComponent(formatManager, transportSource));
	addAndMakeVisible(rightTrack = new TrackComponent(formatManager, transportSource));

	addKeyListener(commandManager.getKeyMappings());
	commandManager.registerAllCommandsForTarget(this);
	toolbar.addKeyListener(this);

	setSize(getParentWidth(), getParentHeight());
	extractButton->setEnabled(false);

	// Audio setup
	setAudioChannels(0, 2);

	mixerSource.addChangeListener(this);
	transportSource.addChangeListener(this);

	transportSource.setSource(&mixerSource);
	sourcePlayer.setSource(&transportSource);

	deviceManager.addAudioCallback(&sourcePlayer);

	setApplicationCommandManagerToWatch(&commandManager);

	formatManager.registerBasicFormats();
    //[/Constructor]
}

MainWindow::~MainWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    helpLabel = nullptr;
    extractButton = nullptr;
    iterations = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	shutdownAudio();
    //[/Destructor]
}

//==============================================================================
void MainWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffa3a3a3));

    //[UserPaint] Add your own custom painting code here..

	// draws menu bar at top of screen ========================
	Rectangle<int> screen = getLocalBounds();
	Rectangle<int> menu = screen.removeFromTop(LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight());
	menuBar->setBounds(menu);
	menuBar->toFront(true);
	// =======================================================

	Rectangle<int> controls = screen.reduced(5);
	Rectangle<int> info = controls.removeFromTop(25);
	helpLabel->setBounds(info.removeFromLeft(getWidth()/2));
	iterations->setBounds(info.removeFromRight(getWidth()/4));

	controls.removeFromTop(10);
	extractButton->setBounds(controls.removeFromTop(30));
	controls.removeFromTop(10);

	leftTrack->setBounds(controls.removeFromTop(leftTrack->getHeight()));
	controls.removeFromTop(10);
	centreTrack->setBounds(controls.removeFromTop(leftTrack->getHeight()));
	controls.removeFromTop(10);
	rightTrack->setBounds(controls.removeFromTop(leftTrack->getHeight()));

	controls.removeFromTop(10);
	toolbar.setBounds(controls.removeFromTop(30).removeFromLeft(170));

    //[/UserPaint]
}

void MainWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    helpLabel->setBounds (16, 8, getWidth() - 252, 24);
    extractButton->setBounds (8, 40, getWidth() - 16, 24);
    iterations->setBounds (296, 8, proportionOfWidth (0.3266f), 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainWindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == extractButton)
    {
        //[UserButtonCode_extractButton] -- add your button handler code here..

		for (int i = 1; i <= NUM_SOURCES; i++)
		{
			inverseTransform(i);
			write(i);
		}

        //[/UserButtonCode_extractButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void MainWindow::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == iterations)
    {
        //[UserComboBoxCode_iterations] -- add your combo box handling code here..
		extractor->numIters = iterations->getText().getIntValue();
        //[/UserComboBoxCode_iterations]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// load file into program and perform fft
bool MainWindow::loadFile(File& file)
{

	ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(file);

	if ((reader->numChannels < 2) || (reader->lengthInSamples < (WINDOW_SIZE * 10)))
		return (fileLoaded = false);

	int numChannels = reader->numChannels;
	int length = reader->lengthInSamples;

	AudioSampleBuffer sampleBuffer(numChannels, length);
	sampleBuffer.clear();
	noOfCols = 1 + floor((length - WINDOW_SIZE) / HOP_SIZE);
	int startSample = 0;
	comp_L = {};
	comp_R = {};

	extractor->initSources(noOfCols);

	//loop and read file into buffer
	for (int col = 0; col < noOfCols; col++)
	{
		sampleBuffer.clear();
		reader->read(&sampleBuffer, 0, sampleBuffer.getNumSamples(), startSample, true, true);

		// get channel data from sampleBuffer
		bufferData_L = (float*)sampleBuffer.getReadPointer(0, 0);
		bufferData_R = (float*)sampleBuffer.getReadPointer(1, 0);

		stft_L.initWindow(1);
		stft_L.applyWindowFunction(bufferData_L);
		comp_L = stft_L.realToComplex(stft_L.performForwardTransform(bufferData_L));

		stft_R.initWindow(1);
		stft_R.applyWindowFunction(bufferData_R);
		comp_R = stft_R.realToComplex(stft_R.performForwardTransform(bufferData_R));

		extractor->extractSources(comp_L, comp_R);
		extractor->synthesize(col);

		startSample += HOP_SIZE;
	}

	return (fileLoaded = true);
}

// resynthesize a source for export to audio
void MainWindow::inverseTransform(int direction)
{
	int numSamps = ((noOfCols - 1) * HOP_SIZE) + WINDOW_SIZE;
	istft_L.initWindow(1);

	float temp_L[WINDOW_SIZE] = {};
	float temp_R[WINDOW_SIZE] = {};
	MatrixXf realMat_L = MatrixXf::Zero(WINDOW_SIZE, noOfCols);
	MatrixXf realMat_R = MatrixXf::Zero(WINDOW_SIZE, noOfCols);

	switch (direction)
	{
	case LEFT:
		realMat_L = istft_L.complexToReal(extractor->leftSource_L);
		realMat_R = istft_R.complexToReal(extractor->leftSource_R);
		break;
	case CENTRE:
		realMat_L = istft_L.complexToReal(extractor->centreSource_L);
		realMat_R = istft_R.complexToReal(extractor->centreSource_R);
		break;
	case RIGHT:
		realMat_L = istft_L.complexToReal(extractor->rightSource_L);
		realMat_R = istft_R.complexToReal(extractor->rightSource_R);
		break;
	}

	for (int i = 0; i < numSamps; i++) // initialise signal array
	{
		signal_left.set(i, 0.f);
		signal_right.set(i, 0.f);
	}

	// ====================== ADD-OVERLAP ========================

	int offset = 0;
	for (int col = 0; col < noOfCols; col++)
	{
		for (int row = 0; row < WINDOW_SIZE; row++)
		{
			temp_L[row] = realMat_L(row, col);
			temp_R[row] = realMat_R(row, col);
		}

		istft_L.performInverseTransform(temp_L, ifftResults_L);
		istft_L.rescale(ifftResults_L);
		istft_R.performInverseTransform(temp_R, ifftResults_R);
		istft_R.rescale(ifftResults_R);

		for (int i = 0; i < WINDOW_SIZE; i++)
		{
			signal_left.set(offset + i, (signal_left[offset + i] + (ifftResults_L[i] * istft_L.window[i])));
			signal_right.set(offset + i, (signal_right[offset + i] + (ifftResults_R[i] * istft_L.window[i])));
		}

		offset += HOP_SIZE;

	// =======================================================
	}

}

// write the resynthesized source to a wave audio file
void MainWindow::write(int direction)
{
	float gain = 1.0f / 1.5f;
	int size = signal_left.size();
	FileOutputStream *output;

	AudioSampleBuffer samples(2, size);
	samples.clear();

	const float* leftData = signal_left.getRawDataPointer();
	const float* rightData = signal_right.getRawDataPointer();
	samples.addFrom(0, 0, leftData, samples.getNumSamples(), gain);
	samples.addFrom(1, 0, rightData, samples.getNumSamples(), gain);
	wav = new WavAudioFormat();
	TrackComponent* destTrack;

	if (direction == LEFT)
	{
		outFile = new File(File::getCurrentWorkingDirectory().getChildFile(fileName + "_Left.wav"));
		destTrack = leftTrack;
	}
	else if (direction == CENTRE)
	{
		outFile = new File(File::getCurrentWorkingDirectory().getChildFile(fileName + "_Centre.wav"));
		destTrack = centreTrack;
	}
	else if (direction == RIGHT)
	{
		outFile = new File(File::getCurrentWorkingDirectory().getChildFile(fileName + "_Right.wav"));
		destTrack = rightTrack;
	}

	if (outFile->exists())
		outFile->deleteFile();

	outFile->create();
	output = outFile->createOutputStream();
	AudioFormatWriter* writer = wav->createWriterFor(output, 44100.0, 2, 16, NULL, 0);
	writer->writeFromAudioSampleBuffer(samples, 0, samples.getNumSamples());
	writer->flush();
	delete writer;
	writer = nullptr;
	//outFile->revealToUser();

	destTrack->setFile(*outFile);
	AudioFormatReader* reader = formatManager.createReaderFor(*outFile);
	readerSources.add(new AudioFormatReaderSource(reader, true));

	if (direction == LEFT)
	{
		mixerSource.addInputSource(readerSources.getUnchecked(0), true);
	}
	else if (direction == CENTRE)
	{
		mixerSource.addInputSource(readerSources.getUnchecked(1), true);
	}
	else if (direction == RIGHT)
	{
		mixerSource.addInputSource(readerSources.getUnchecked(2), true);
	}

}


// Methods inherited from MenuBarModel =========================================
StringArray MainWindow::getMenuBarNames()
{
	StringArray names = { "File", "Playback", "Options", "Help" };
	return StringArray(names);
}

PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
{
	PopupMenu menu;

	// 'File' menu
	if (topLevelMenuIndex == 0)
	{
		menu.addCommandItem(&commandManager, load);
		menu.addCommandItem(&commandManager, save);
		menu.addSeparator();
		menu.addCommandItem(&commandManager, quit);
	}
	// 'Options' menu
	else if (topLevelMenuIndex == 1)
	{
		menu.addCommandItem(&commandManager, play);
		menu.addCommandItem(&commandManager, followTransport);
		menu.addCommandItem(&commandManager, pause);
		menu.addCommandItem(&commandManager, stop);
	}
	else if (topLevelMenuIndex == 2)
	{
		menu.addCommandItem(&commandManager, settings);
	}
	// 'Help' menu
	else if (topLevelMenuIndex == 3)
	{
		menu.addCommandItem(&commandManager, about);
	}

	return menu;
}

void MainWindow::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{

}
// =============================================================================

// Methods inherited from ApplicationCommandTarget =============================
ApplicationCommandTarget* MainWindow::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void MainWindow::getAllCommands(juce::Array< CommandID > &commands)
{
	const CommandID ids[] = { load, save, quit, about, settings, play, followTransport, pause, stop };

	commands.addArray(ids, numElementsInArray(ids));
}

void MainWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo &result)
{
	const String menu("Menu");

	switch (commandID)
	{
	case CommandIDs::load:
		result.setInfo("Load file...", "Load a file", menu, 0);
		result.addDefaultKeypress('N', ModifierKeys::commandModifier);
		break;
	case CommandIDs::save:
		result.setInfo("Save as .wav", "Save files", menu, 0);
		result.addDefaultKeypress('S', ModifierKeys::commandModifier);
		break;
	case CommandIDs::play:
		result.setInfo("Play", "Begin playback", menu, 0);
		result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::noModifiers);
		break;
	case CommandIDs::pause:
		result.setInfo("Pause", "Pause playback", menu, 0);
		result.addDefaultKeypress('P', ModifierKeys::commandModifier);
		break;
	case CommandIDs::followTransport:
		result.setInfo("Follow Transport", "Follow transport", menu, 1);
		result.addDefaultKeypress('F', ModifierKeys::commandModifier);
		break;
	case CommandIDs::stop:
		result.setInfo("Stop", "Stops playback", menu, 0);
		result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::commandModifier);
		break;
	case CommandIDs::quit:
		result.setInfo("Quit", "Exits the application", menu, 0);
		result.addDefaultKeypress('Q', ModifierKeys::commandModifier);
		break;
	case CommandIDs::settings:
		result.setInfo("Settings", "Settings", menu, 0);
		result.addDefaultKeypress('T', ModifierKeys::commandModifier);
		break;
	case CommandIDs::about:
		result.setInfo("About", "Info about this app", menu, 0);
		break;
	}
}

bool MainWindow::perform(const InvocationInfo& info)
{
	switch (info.commandID)
	{
	case CommandIDs::load: // load file into memory

		chooser = new FileChooser("Select a file to load...",
			File::getSpecialLocation(File::userMusicDirectory), "*.wav");

		if (chooser->browseForFileToOpen())
		{
			File currentFile = chooser->getResult();
			fileName = currentFile.getFileNameWithoutExtension();

			const ScopedLock sl(lock);
			LoadTask task(*this, &currentFile);

			if (task.runThread())
			{
				helpLabel->setText("Loaded file: " + currentFile.getFileName(), dontSendNotification);
				extractButton->setEnabled(true);
			}
			else
			{
				helpLabel->setText("File could not be loaded.", dontSendNotification);
			}
		}
		break;
	case CommandIDs::save:
		outFile->revealToUser();
		break;
	case CommandIDs::play:

		if (transportSource.isPlaying())
		{
			playing = false;
			transportSource.stop();
		}
		else
		{
			playing = true;
			transportSource.start();
		}

		break;

	case CommandIDs::pause:
		playing = false;
		transportSource.stop();
		break;
	case CommandIDs::stop:
		playing = false;
		transportSource.setPosition(0);
		break;
	case CommandIDs::quit:
		// shutdown application
		closeButtonPressed();
		break;
	case CommandIDs::followTransport:

		if (!following)
			following = true;
		else
			following = false;

		leftTrack->setFollowsTransport(following);
		centreTrack->setFollowsTransport(following);
		rightTrack->setFollowsTransport(following);

		break;
	case CommandIDs::settings:
		settingsComp = new AudioDeviceSelectorComponent(deviceManager,
			0, 0, 2, 2,
			false,
			false,
			true,
			false);

		settingsComp->setSize(500, 400);
		DialogWindow::showModalDialog(String("Audio Settings"),
			settingsComp,
			TopLevelWindow::getTopLevelWindow(0),
			Colours::white,
			true);
		break;

	case CommandIDs::about:
		aboutComp = new About();
		aboutComp->setSize(300, 100);
		DialogWindow::showModalDialog("About",
			aboutComp,
			TopLevelWindow::getTopLevelWindow(0),
			Colours::green,
			true);
		break;

	default:
		return false;
	}

	return true;
}

void MainWindow::closeButtonPressed()
{
	JUCEApplication::getInstance()->systemRequestedQuit();
}
// =============================================================================

// Methods inherited from AudioAppComponent ====================================
void MainWindow::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{}

void MainWindow::getNextAudioBlock(const juce::AudioSourceChannelInfo & channelInfo)
{}

void MainWindow::releaseResources()
{}
// =============================================================================

void MainWindow::changeListenerCallback(ChangeBroadcaster* broadcastSource)
{
	if (broadcastSource == &transportSource)
	{
		DBG("Transport callback!");
	}
}

bool MainWindow::keyPressed(const KeyPress& key, Component* originatingComponent)
{
	return true;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainWindow" componentName="mainWindow"
                 parentClasses="public AudioAppComponent, public MenuBarModel, public ApplicationCommandTarget, public KeyListener, public ChangeListener"
                 constructorParams="" variableInitialisers="extractor(new PanExtractor()), panDirection(LEFT), stft_L(WINDOW_SIZE), stft_R(WINDOW_SIZE)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="300" initialHeight="300">
  <BACKGROUND backgroundColour="ffa3a3a3"/>
  <LABEL name="help" id="f623bed68eed7e56" memberName="helpLabel" virtualName=""
         explicitFocusOrder="0" pos="16 8 252M 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Drag an audio file to load..." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="extract" id="1d3618be20925b35" memberName="extractButton"
              virtualName="" explicitFocusOrder="0" pos="8 40 16M 24" tooltip="Extract sources from audio file."
              bgColOff="ffcdffbb" buttonText="Extract Sources" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="iterations" id="cf2534eac3753608" memberName="iterations"
            virtualName="" explicitFocusOrder="0" pos="296 8 32.615% 24"
            editable="1" layout="33" items="" textWhenNonSelected="No. of iterations"
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
