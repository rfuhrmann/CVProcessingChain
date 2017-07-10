//============================================================================
// Name        : KeypointMatcher.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for KeypointMatching steps
//============================================================================

#include <iostream>
#include<list>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class KeypointMatcher {

public:
	// constructor
	KeypointMatcher(void) {};
	// destructor
	~KeypointMatcher(void) {};

	vector<DMatch> ratioMatcher(int type, Mat& descriptors1, Mat& descriptors2);
	
	void thresholdFilter(int thresh, vector<DMatch>& matches);
	void ransacFilter(vector<KeyPoint>& keypointsObject, vector<KeyPoint>& keypointsScene, vector<DMatch>& matches);
	float homographyFilter(vector<KeyPoint>& keypoints1, vector<KeyPoint>& keypoints2, vector<DMatch>& matches, Mat H);

private:


};
