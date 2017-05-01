//============================================================================
// Name        : KeypointDetetion.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for KeypointDetection steps
//============================================================================

#include <iostream>
#include<list>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class KeypointDetection {

public:
	// constructor
	KeypointDetection(void){};
	// destructor
	~KeypointDetection(void){};
	void test(void);
	vector<Mat> run(Mat&);
	void surf(Mat img);
	void mser(Mat img);
	void brisk(Mat img);
	void freak(Mat img);

private:


};
