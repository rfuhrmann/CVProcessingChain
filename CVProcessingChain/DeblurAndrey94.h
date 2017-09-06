//============================================================================
// Name        : DeblurAndrey94.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for CVProcessingChain Deblurring
//============================================================================

#include <iostream>
#include <list>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class DeblurAndrey94
{
public:
	DeblurAndrey94(){};
	~DeblurAndrey94(){};

	Mat deblur(Mat& img);



private:
	Mat erosion(const Mat &grayImage, int erosionSize);

	Mat sharpImage(const Mat &grayImage, float sigmar);

	//TO-DO try Fast Noise Variance Estimation JOHN IMMERKÆR
	float getInvSNR(const Mat &grayBlurred);

	float measureBlur(const Mat &grayBlurred);

	bool isBlurred(const Mat &grayBlurred);

	Mat getAutoCerrelation(const Mat &blurred);

	void cropBorder(Mat &thresholded);

	int estimateKernelSize(const Mat &grayBlurred);

	void computeDFT(const Mat &image, Mat *result);

	void computeIDFT(Mat *input, Mat &result);

	void rotate(Mat &src, Mat &dst);

	void applyConstraints(Mat &image, float thresholdValue);

	void normalizePSF(Mat &image);

	void blindDeblurring(const Mat &blurred, Mat &deblurred, Mat &kernel, int iters);

	void blindDeblurringOneChannel(const Mat &blurred, Mat &kernel, int kernelSize, int iters, float noisePower);

	void wienerFilter(const Mat &blurredImage, const Mat &known, Mat &unknown, float noisePower);

};

