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
	vector<Mat> run(Mat&, const char* imageName, bool showImage, bool writeFile);
	vector<KeyPoint> surf(Mat& img, const char* imageName, bool showImage, bool writeFile);
	vector<KeyPoint> mser(Mat& img, const char* imageName, bool showImage, bool writeFile);
	vector<KeyPoint> brisk(Mat& img, const char* imageName, bool showImage, bool writeFile);
	vector<KeyPoint> freak(Mat& img, const char* imageName, bool showImage, bool writeFile);
	vector<KeyPoint> orb(Mat& img, const char* imageName, bool showImage, bool writeFile);
	void printKeypoints(vector<KeyPoint> kp);
	void writeKeypoints(const char* filename, vector<KeyPoint> kp);// , String filename);
	void showKeypoints(const char* win, Mat& img, bool cut = true);
private:


};
