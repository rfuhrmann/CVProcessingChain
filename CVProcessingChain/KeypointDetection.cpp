//============================================================================
// Name        : KeypointDetection.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : uses keypoint detectors of the opencv framework to find keypoints
//============================================================================

#include "KeypointDetection.h"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

 void KeypointDetection::test() {
	cout << "inside KeypointDetection ..." << endl;

}

 // function calls keypoint detectors functions
 /*
 in                   :  input image
 return               :  vector of input image and all keypoint images
 */
vector<Mat> KeypointDetection::run(Mat& img) {
	vector<Mat> images;
	images.push_back(img);
	//images.push_back(surf(img));
	surf(img);
	return images;
}

// surf keypoint detection
// speeded up robust features
/*
img         :  input image
return      :  the result image
*/
void KeypointDetection::surf(Mat img) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);
	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Draw keypoints
	drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	//-- Show detected (drawn) keypoints
	//imshow("Keypoints 1", img2);
	//waitKey(0);
}

// mser keypoint detection
// Maximally Stable Extremal Regions
/*
img         :  input image
return      :  the result image
*/
void KeypointDetection::mser(Mat img) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);

	//########## extendet version ##########
	//vector< vector< Point> > contours;
	//vector< Rect> bboxes;
	//Ptr<MSER> mser = MSER::create(21, (int)(0.00002*img2.cols*img2.rows), (int)(0.05*img2.cols*img2.rows), 1, 0.7);
	//mser->detectRegions(img2, contours, bboxes);

	//for (int i = 0; i < bboxes.size(); i++)
	//{
	//	rectangle(img2, bboxes[i], CV_RGB(0, 255, 0));
	//}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	Ptr<MSER> detector = MSER::create();
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Draw keypoints
	drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//namedWindow("mser");
	//imshow("mser", img2);
	//waitKey(0);
}


// mser keypoint detection
//Binary Robust Invariant Scalable Keypoints
/*
img         :  input image
return      :  the result image
*/
void KeypointDetection::brisk(Mat img) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);
	//-- Step 1: Detect the keypoints using SURF Detector
	Ptr<BRISK> detector = BRISK::create();
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Draw keypoints
	drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	//-- Show detected (drawn) keypoints
	//imshow("brisk", img2);
	//waitKey(0);
}

// freak keypoint detection
//Fast Retina Keypoint
/*
img         :  input image
return      :  the result image
*/
void KeypointDetection::freak(Mat img) {
	//Mat img2 = img.clone();
	//img2.convertTo(img2, CV_8UC1);
	////-- Step 1: Detect the keypoints using SURF Detector
	//Ptr<FREAK> detector = FREAK::create();
	//vector<KeyPoint> keypoints;
	//detector->detect(img2, keypoints);

	////-- Draw keypoints
	//drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	////-- Show detected (drawn) keypoints
	//imshow("freak", img2);
	//waitKey(0);
}