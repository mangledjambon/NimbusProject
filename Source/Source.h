/*
  ==============================================================================

    Source.h
    Created: 3 Jun 2015 11:47:45am
    Author:  R00070693

  ==============================================================================
*/

#ifndef SOURCE_H_INCLUDED
#define SOURCE_H_INCLUDED

#include "Constants.h"
#include "Eigen\Eigen"
using Eigen::MatrixXcf;

class Source
{

public:
	Source(){}
	~Source(){}
	MatrixXcf left = MatrixXcf((WINDOW_SIZE / 2) + 1, 2);
	MatrixXcf right = MatrixXcf((WINDOW_SIZE / 2) + 1, 2);
	MatrixXcf centre = MatrixXcf((WINDOW_SIZE / 2) + 1, 2);
};

#endif  // SOURCE_H_INCLUDED
