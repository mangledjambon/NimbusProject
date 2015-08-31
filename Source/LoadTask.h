/*
  ==============================================================================

    LoadTask.h
    Created: 22 Jun 2015 10:58:14am
    Author:  R00070693

	Launches a thread to handle loading file with progress bar

  ==============================================================================
*/

#ifndef LOADTASK_H_INCLUDED
#define LOADTASK_H_INCLUDED

#include "JuceHeader.h"
#include "MainWindow.h"
class MainWindow;

class LoadTask : public ThreadWithProgressWindow
{
public:

	/*
		Constructor
		input: MainWindow (parent), file to load
	*/
	LoadTask(MainWindow& window, File* file);

	~LoadTask();

	/*
		implementation of ThreadWithProgessWindow method
	*/
	void run() override;

	/*
	implementation of ThreadWithProgessWindow method
	*/
	void threadComplete(bool) override;

private:
	MainWindow* window;
	File* file;
	bool userPressedCancel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoadTask);
};

#endif  // LOADTASK_H_INCLUDED
