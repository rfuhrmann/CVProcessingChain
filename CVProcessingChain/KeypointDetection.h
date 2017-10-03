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
	//vector<Mat> run(Mat&, const char* imageName, bool showImage, bool writeFile);
	vector<KeyPoint> sift(Mat& img, const char* imageName, bool showImage);
	vector<KeyPoint> surf(Mat& img, const char* imageName, bool showImage);
	vector<KeyPoint> brisk(Mat& img, const char* imageName, bool showImag);
	vector<KeyPoint> fast(Mat& img, const char* imageName, bool showImage);
	vector<KeyPoint> orb(Mat& img, const char* imageName, bool showImage);
	void printKeypoints(vector<KeyPoint> kp);
	void writeKeypoints(const char* filename, vector<KeyPoint> kp);// , String filename);
	void showKeypoints(const char* win, Mat& img, bool cut = true);
private:


};
