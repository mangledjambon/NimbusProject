/*
  ==============================================================================

    LoadTask.cpp
    Created: 22 Jun 2015 10:58:14am
    Author:  R00070693

  ==============================================================================
*/
#include "LoadTask.h"

LoadTask::LoadTask(MainWindow& w, File* f) : ThreadWithProgressWindow(String("Loading..."), true, false, 4000, String("Cancel")), userPressedCancel(false)
{
	window = &w;
	file = f;
	setStatusMessage("Preparing audio file for extraction. This can take a while...");
}

LoadTask::~LoadTask()
{
}

void LoadTask::run()
{
	setProgress(-1.0);

	if (threadShouldExit())
		return;

	window->loadFile(*file);
	
}

void LoadTask::threadComplete(bool userPressedCancel)
{
	if (userPressedCancel)
	{
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"Load file",
			"You pressed cancel!");
	}
	else
	{
		// thread finished normally..
		AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
			"Complete.",
			"File successfully loaded!");
	}
}