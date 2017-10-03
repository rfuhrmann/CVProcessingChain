//============================================================================
// Name        : KeypointDescription.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : uses keypoint descriptors of the opencv framework to find keypoint descriptions
//============================================================================

#include "KeypointDescription.h"
#include "FileManager.h"
#include "KeypointMatcher.h"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;


//###################################### single image #################################################

// sift keypoint description
// scale-invariant feature transform
/*
img         :  input image
kp			:  keypoints which should be described
return      :  the result image
*/
Mat KeypointDescription::sift(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<SIFT> detector = SIFT::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	return descriptors;
}

// surf keypoint description
// speeded up robust features
/*
img         :  input image
kp			:  keypoints which should be described
return      :  the result image
*/
Mat KeypointDescription::surf(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<SURF> detector = SURF::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	return descriptors;
}

// freak keypoint description
// fast retina keypoint
/*
img         :  input image
kp			:  keypoints which should be described
return      :  the result image
*/
Mat KeypointDescription::freak(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<FREAK> detector = FREAK::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	return descriptors;
}

// brisk keypoint description
// Binary Robust Invariant Scalable Keypoints
/*
img         :  input image
kp			:  keypoints which should be described
return      :  the result image
*/
Mat KeypointDescription::brisk(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<BRISK> detector = BRISK::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	return descriptors;
}

// orb keypoint description
//Oriented FAST and Rotated BRIEF
/*
img         :  input image
kp			:  keypoints which should be described
return      :  the result image
*/
Mat KeypointDescription::orb(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<ORB> detector = ORB::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	return descriptors;
}

