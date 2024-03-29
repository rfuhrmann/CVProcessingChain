//============================================================================
// Name        : KeypointDetection.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : uses keypoint detectors of the opencv framework to find keypoints
//============================================================================

#include "KeypointDetection.h"
#include "FileManager.h"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

// sift keypoint detection
// scale-invariant feature transform
/*
img         :  input image
imageName	:  for imshow window
showImage	:  if image should be shown
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::sift(Mat& img, const char* imageName, bool showImage) {
	Mat img2 = img.clone();
	Ptr<SIFT> detector = SIFT::create();
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Show detected (drawn) keypoints
	if (showImage) {
		drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		imshow("Keypoints: " + String(imageName) + "_SIFT", img2);
	}
	return keypoints;
}

// surf keypoint detection
// speeded up robust features
/*
img         :  input image
imageName	:  for imshow window
showImage	:  if image should be shown
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::surf(Mat& img, const char* imageName, bool showImage) {
	Mat img2 = img.clone();
	Ptr<SURF> detector = SURF::create(100);
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);
	
	//-- Show detected (drawn) keypoints
	if (showImage) {
		drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		imshow("Keypoints: " + String(imageName) + "_SURF", img2);
	}
	return keypoints;
}

// fast keypoint detection
// features from accelerated segment test
/*
img         :  input image
imageName	:  for imshow window
showImage	:  if image should be shown
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::fast(Mat& img, const char* imageName, bool showImage) {
	Mat img2 = img.clone();
	vector<KeyPoint> keypoints;
	FAST(img2, keypoints, 40, true);
	
	//-- Show detected (drawn) keypoints
	if (showImage) {
		drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		imshow("Keypoints: " + String(imageName) + "_FAST", img2);
	}
	return keypoints;
}


// brisk keypoint detection
// Binary Robust Invariant Scalable Keypoints
/*
img         :  input image
imageName	:  for imshow window
showImage	:  if image should be shown
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::brisk(Mat& img, const char* imageName, bool showImage) {
	Mat img2 = img.clone();
	Ptr<BRISK> detector = BRISK::create();
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Show detected (drawn) keypoints
	if (showImage) {
		drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		imshow("Keypoints: " + String(imageName) + "_BRISK", img2);
	}
	return keypoints;
}

// orb keypoint detection
// Oriented FAST and Rotated BRIEF
/*
img         :  input image
imageName	:  for imshow window
showImage	:  if image should be shown
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::orb(Mat& img, const char* imageName, bool showImage) {
	Mat img2 = img.clone();
	Ptr<ORB> detector = ORB::create(1000);
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Show detected (drawn) keypoints
	if (showImage) {
		drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		imshow("Keypoints: " + String(imageName) + "_ORB", img2);
	}
	return keypoints;
}

// Function prints keypoints to the std::cout (console)
/*
kp		   :  keypoints for printing
*/
void KeypointDetection::printKeypoints(vector<KeyPoint> kp) {
	cout << "surf keypoints ...";
	int kpSize = kp.size();
	for (int i = 0; i < kpSize; i++) {
		cout << kp[i].pt.x << "-" << kp[i].pt.y << ", ";
	}
	cout << endl;
}

// Function write create or overwrite a file and write keypoints
/*
filename   :  filename for writing
kp		   :  keypoints for writing
*/
void KeypointDetection::writeKeypoints(const char* filename, vector<KeyPoint> kp) {
	int kpSize = kp.size();
	if(kpSize < 1) return;
	ofstream(String(filename));
	fstream f;
	f.open(String(filename), ios::out);
	for (int i = 0; i < kpSize-1; i++) {
		f << kp[i].pt.x << "-" << kp[i].pt.y << ",";
	}
	f << kp[kpSize-1].pt.x << "-" << kp[kpSize-1].pt.y;
	f.close();
}

// Function displays image with keypoints (after proper normalization)
/*
win   :  Window name
img   :  Image that shall be displayed
cut   :  whether to cut or scale values outside of [0,255] range
*/
void KeypointDetection::showKeypoints(const char* win, Mat& img, bool cut) {
	Mat tmp = img.clone();
	if (tmp.channels() == 1) {
		if (cut) {
			threshold(tmp, tmp, 255, 255, CV_THRESH_TRUNC);
			threshold(tmp, tmp, 0, 0, CV_THRESH_TOZERO);
		}
		else
			normalize(tmp, tmp, 0, 255, CV_MINMAX);
		tmp.convertTo(tmp, CV_8UC1);
	}
	else {
		tmp.convertTo(tmp, CV_8UC3);
	}
	imshow(win, tmp);
}