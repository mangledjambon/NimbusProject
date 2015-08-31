/*
  ==============================================================================

    PanExtractor.cpp
    Created: 28 May 2015 11:41:39am
    Author:  R00070693

  ==============================================================================
*/
#include "PanExtractor.h"
#include "Constants.h"

PanExtractor::PanExtractor() 	:	pan_dir(0, (const float)M_PI / 4, (const float)M_PI / 2),
									projmat(3, 2),
									panmat(3, 2),
									k(3, 3),
									kAbs(3, 3),
									pimat(3, 2),
									cAbs((WINDOW_SIZE / 2) + 1, 3),
									cc((WINDOW_SIZE / 2) + 1, 3),
									ce((WINDOW_SIZE / 2) + 1, 3),
									ck((WINDOW_SIZE / 2) + 1, 3),
									s(MatrixXf::Random((WINDOW_SIZE / 2) + 1, 3)),
									sAbs((WINDOW_SIZE / 2) + 1, 3),
									est((WINDOW_SIZE / 2) + 1, 3),
									ei((WINDOW_SIZE / 2) + 1, 3),
									uS((WINDOW_SIZE / 2) + 1, 3),
									dS((WINDOW_SIZE / 2) + 1, 3),
									x((WINDOW_SIZE / 2) + 1, 2),
									yp((WINDOW_SIZE / 2) + 1, 2)


{
	// constructor

	// initialisation for audio extraction
	projmat << pan_dir.array().sin(), -(pan_dir.array().cos());
	panmat << pan_dir.array().cos(), pan_dir.array().sin();
	k = panmat * projmat.transpose();
	kAbs << k.cwiseAbs();
	k.resize(0, 0);
	kSum = kAbs.colwise().sum().transpose();
	kSum.transposeInPlace();
	lS = kSum.replicate(2049, 1);
	lS.array() + eps;
	Matrix3f ident = Matrix3f::Identity(3, 3);
	pimat = projmat.fullPivHouseholderQr().solve(ident);
	sAbs << s.cwiseAbs();

}

PanExtractor::~PanExtractor()
{
	// destructor
}

void PanExtractor::extractSources(std::complex<float>* left, std::complex<float>* right)
{

	// loop over 2 columns, 2049 samples each
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < (WINDOW_SIZE / 2) + 1; i++)
		{
			if (j == 0)
			{
				// load left into x (matrix for fft'd data)
				x(i, j) = left[i];
			}
			else if (j == 1)
			{
				// load right into x
				x(i, j) = right[i];
			}
		}
	}

	cc = x * projmat.transpose();
	cAbs = cc.cwiseAbs();
	sAbs << s.cwiseAbs();
	//sAbs.array() + 1;

	for (int i = 0; i < numIters; i++)
	{
		i;
		// est=S*K;
		est << sAbs * kAbs;
		//est.array() + eps;

		// ei=C./(est);
		ei << cAbs.cwiseQuotient(est);

		// uS=ei*K';
		uS << ei * kAbs.transpose();

		// dS = (uS./lS).^2;
		dS = uS.cwiseQuotient(lS);
		dS = dS.cwiseProduct(dS);

		// S=S.*dS;
		sAbs = sAbs.cwiseProduct(dS);

	}

}

// resynthesize based on pseudoinverse of each source
void PanExtractor::synthesize(int currentCol)
{

	est = sAbs * kAbs;
	est.array() + eps;

	ce = cc.cwiseQuotient(est);

	for (int i = 0; i < NUM_SOURCES; i++)
	{
		MatrixXf sCol = sAbs.col(i);
		MatrixXf kRow = kAbs.row(i);
		ck = ce.cwiseProduct((sAbs.col(i) * kAbs.row(i)));

		yp = ck * pimat.transpose();
		int rows = yp.rows();

		switch (i)
		{
		case 0: 	
			for (int row = 0; row < rows; row++)
			{
				leftSource_L(row, currentCol) = yp(row, 0);
				leftSource_R(row, currentCol) = yp(row, 1);
			}
			break;
		case 1: 
			for (int row = 0; row < rows; row++)
			{
				centreSource_L(row, currentCol) = yp(row, 0);
				centreSource_R(row, currentCol) = yp(row, 1);
			}
			break;
		case 2: 
			for (int row = 0; row < rows; row++)
			{
				rightSource_L(row, currentCol) = yp(row, 0);
				rightSource_R(row, currentCol) = yp(row, 1);
			}
			break;
		}
	}
}

// initialise Left and Right matrices for each source with zeros
void PanExtractor::initSources(int cols)
{
	const int COMPLEX_SIZE = (WINDOW_SIZE / 2) + 1;

	leftSource_L = MatrixXcf::Zero(COMPLEX_SIZE, cols);
	leftSource_R = MatrixXcf::Zero(COMPLEX_SIZE, cols);
	centreSource_L = MatrixXcf::Zero(COMPLEX_SIZE, cols);
	centreSource_R = MatrixXcf::Zero(COMPLEX_SIZE, cols);
	rightSource_L = MatrixXcf::Zero(COMPLEX_SIZE, cols);
	rightSource_R = MatrixXcf::Zero(COMPLEX_SIZE, cols);
}
