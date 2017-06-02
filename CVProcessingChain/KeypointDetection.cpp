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
//#include "windows.h"

using namespace cv;
using namespace cv::xfeatures2d;

 void KeypointDetection::test() {
	//cout << "inside KeypointDetection ..." << endl;

}

 // function calls keypoint detectors functions
 /*
 in                   :  input image
 return               :  vector of input image and all keypoint images
 */
vector<Mat> KeypointDetection::run(Mat& img, const char* imageName, bool showImage, bool writeFile) {
	vector<Mat> images;
	images.push_back(img);
	//images.push_back(surf(img));
	surf(img, imageName, showImage, writeFile);
	mser(img, imageName, showImage, writeFile);
	return images;
}

// surf keypoint detection
// speeded up robust features
/*
img         :  input image
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::surf(Mat& img, const char* imageName, bool showImage, bool writeFile) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);
	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Draw keypoints
	//printKeypoints(keypoints);
	//cout << "#kp ..." << keypoints.size() <<endl;
	drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	//-- Show detected (drawn) keypoints
	if (showImage) imshow("Keypoints: "+String(imageName)+"_SURF", img2);
	//-- (over)Write keypoints to file (create file if non exists)
	if (writeFile) {
		string filename = imageName;
		//filename += "_surf.dat";
		writeKeypoints(filename.c_str(), keypoints);
	}
	//write keypoints to json document
	FileManager fileManager;
	fileManager.writeKeypointsToJson(imageName, keypoints);
	return keypoints;
}

// mser keypoint detection
// Maximally Stable Extremal Regions
/*
img         :  input image
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::mser(Mat& img, const char* imageName, bool showImage, bool writeFile) {
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
	//int minHessian = 400;
	Ptr<MSER> detector = MSER::create();
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Draw keypoints
	drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//-- Show detected (drawn) keypoints
	if (showImage) imshow("Keypoints: " + String(imageName) + "_MSER", img2);
	//-- (over)Write keypoints to file (create file if non exists)
	if (writeFile) {
		string filename = imageName;
		//filename += "_mser.dat";
		writeKeypoints(filename.c_str(), keypoints);
	}
	//write keypoints to json document
	FileManager fileManager;
	fileManager.writeKeypointsToJson(imageName, keypoints);
	return keypoints;
}


// mser keypoint detection
//Binary Robust Invariant Scalable Keypoints
/*
img         :  input image
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::brisk(Mat& img, const char* imageName, bool showImage, bool writeFile) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);
	//-- Step 1: Detect the keypoints using SURF Detector
	Ptr<BRISK> detector = BRISK::create();
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Draw keypoints
	drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//-- Show detected (drawn) keypoints
	if (showImage) imshow("Keypoints: " + String(imageName) + "_BRISK", img2);
	//-- (over)Write keypoints to file (create file if non exists)
	if (writeFile) {
		string filename = imageName;
		//filename += "_brisk.dat";
		writeKeypoints(filename.c_str(), keypoints);
	}
	//write keypoints to json document
	FileManager fileManager;
	fileManager.writeKeypointsToJson(imageName, keypoints);
	return keypoints;
}

// freak keypoint detection
// Fast REtinA Keypoint
/*
img         :  input image
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::freak(Mat& img, const char* imageName, bool showImage, bool writeFile) {
	//Mat img2 = img.clone();
	//img2.convertTo(img2, CV_8UC1);
	////-- Step 1: Detect the keypoints using SURF Detector
	//Ptr<FREAK> detector = FREAK::create();
	vector<KeyPoint> keypoints;
	//detector->detect(img2, keypoints);

	//-- Show detected (drawn) keypoints
	//if (showImage) imshow("Keypoints: " + String(imageName) + "_FREAK", img2);
	////-- (over)Write keypoints to file (create file if non exists)
	//if (writeFile) {
	//	string filename = imageName;
	//	filename += "_freak.dat";
	//	writeKeypoints(filename.c_str(), keypoints);
	//}
	return keypoints;
}
  
// orb keypoint detection
//Oriented FAST and Rotated BRIEF
/*
img         :  input image
return      :  the result image
*/
vector<KeyPoint> KeypointDetection::orb(Mat& img, const char* imageName, bool showImage, bool writeFile) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);
	//-- Step 1: Detect the keypoints using SURF Detector
	Ptr<ORB> detector = ORB::create();
	vector<KeyPoint> keypoints;
	detector->detect(img2, keypoints);

	//-- Draw keypoints
	drawKeypoints(img2, keypoints, img2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//-- Show detected (drawn) keypoints
	if (showImage) imshow("Keypoints: " + String(imageName) + "_ORB", img2);
	//-- (over)Write keypoints to file (create file if non exists)
	if (writeFile) {
		string filename = imageName;
		writeKeypoints(filename.c_str(), keypoints);
	}
	//write keypoints to json document
	FileManager fileManager;
	fileManager.writeKeypointsToJson(imageName, keypoints);
	return keypoints;
}

// Function prints keypoints to the std::cout (console)
/*
kp		   :  keypoints
*/
void KeypointDetection::printKeypoints(vector<KeyPoint> kp) {
	//cout << "anz kp ..."<<kp.size()<<endl;
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
kp		   :  keypoints
*/
void KeypointDetection::writeKeypoints(const char* filename, vector<KeyPoint> kp) {
	int kpSize = kp.size();
	if(kpSize < 1) return;
	ofstream(String(filename));// .c_str());
	fstream f;
	f.open(String(filename)/*.c_str()*/, ios::out);
	for (int i = 0; i < kpSize-1; i++) {
		f << kp[i].pt.x << "-" << kp[i].pt.y << ",";
	}
	f << kp[kpSize-1].pt.x << "-" << kp[kpSize-1].pt.y;
	f.close();
	//cout << " > writing " << filename/*.c_str()*/ << " done." << endl;
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