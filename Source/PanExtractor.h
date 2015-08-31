/*
  ==============================================================================

    PanExtractor.h
    Created: 28 May 2015 11:41:39am
    Author:  R00070693

  ==============================================================================
*/

#ifndef PANEXTRACTOR_H_INCLUDED
#define PANEXTRACTOR_H_INCLUDED

#include "Eigen\Eigen"
using namespace Eigen;

class PanExtractor {

public:

	/*
		Constructor
		Performs some preprocessing for extraction of audio sources
	*/
	PanExtractor();

	~PanExtractor();

	/*
		extracts the sources from the audio data
		input: complex float arrays for left and right channels of stereo audio file
		output: none
	*/
	void extractSources(std::complex<float>* stereo_left, std::complex<float>* stereo_right);

	/*
		resynthesize spectrogram data based on pseudoinverse
		input: column of spectrogram in which to insert new resynthesized audio data
		output: none
	*/
	void synthesize(int currentCol);

	/*
		initialise the source matrices
		input: no of columns needed
		output: none
	*/
	void initSources(int cols);

	// matrices for actual sources
	MatrixXcf leftSource_L, leftSource_R;
	MatrixXcf centreSource_L, centreSource_R;
	MatrixXcf rightSource_L, rightSource_R;
	
	int numIters;

private:
	int noOfColumns;
	Vector3f pan_dir;
	MatrixXf projmat, panmat, kAbs, k, pimat, cAbs, kSum, lS, s, sAbs;
	MatrixXf est, ei, uS, dS;
	MatrixXcf cc, ce, ck, x, yp;	
	
};



#endif  // PANEXTRACTOR_H_INCLUDED
