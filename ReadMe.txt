=================================================
|\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
============== PanRev - ReadMe.txt ==============
|///////////////////////////////////////////////|
=================================================
Author: 	Sean Breen (sean.breen@mycit.ie)
Date:		18/8/2015
Version: 	1.1

PanRev is an application to extract and view the pan information from stereo audio files.

Usage:

1. 	Choose the amount of iterations to use.
	A higher number of iterations will produce better stereo separation, but will take longer to process.  

2.	Load a file with the menu ('File' -> 'Load file...'). This will load a file for processing.
	It must be a STEREO audio file in wav format.
	This may take some time depending on the length of the file chosen, and the amount of iterations used.

3.	When the file has loaded, click the 'Extract Sources' button.
	This will take the separated stereo information from the audio file and display it as three different waveforms.
	What you see in the waveforms is the audio information from the Left, Centre and Right directions (in that order).
	
4	If you wish to use the audio files for another reason outside the application, the files containing the separated 
	information will be in the same directory as the executable. These can be loaded 
	into any audio player/workstation that supports wave format.	
	
If you wish to load another file for extraction, at the moment you must exit the application and start it again.