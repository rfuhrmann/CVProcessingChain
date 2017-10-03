//============================================================================
// Name        : KeypointDescription.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for KeypointDescription steps
//============================================================================

#include <iostream>
#include <list>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class KeypointDescription {

public:
	// constructor
	KeypointDescription(void) {};
	// destructor
	~KeypointDescription(void) {};

	Mat sift(Mat& img, vector<KeyPoint> kp);
	Mat surf(Mat& img, vector<KeyPoint> kp);
	Mat orb(Mat& img, vector<KeyPoint> kp);
	Mat brisk(Mat& img, vector<KeyPoint> kp);
	Mat freak(Mat& img, vector<KeyPoint> kp);


private:

};
