//============================================================================
// Name        : KeypointMatcher.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : uses keypoint matchers of the opencv framework to match keypoints
//============================================================================

#include "KeypointMatcher.h"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;


// match keypoints with crosscheck filter
/*
type				 :  type of distance between two keypoints, used types are: norm_l2, norm_hamming
descriptors1         :  descriptor for keypoints from image 1
descriptors2         :  descriptor for keypoints from image 2
return				 :  matches which could be found and passed the filter
*/
vector<DMatch> KeypointMatcher::crosscheckMatcher(int type, Mat& descriptors1, Mat& descriptors2) {
	vector<DMatch> goodMatches;
	BFMatcher matcher(type, true);
	matcher.match(descriptors1, descriptors2, goodMatches);
	return goodMatches;
}

// match keypoints with ratio filter
/*
type				 :  type of distance between two keypoints, used types are: norm_l2, norm_hamming
descriptors1         :  descriptor for keypoints from image 1
descriptors2         :  descriptor for keypoints from image 2
return				 :  matches which could be found and passed the filter
*/
vector<DMatch> KeypointMatcher::ratioMatcher(int type, Mat& descriptors1, Mat& descriptors2) {
	vector<vector<DMatch>> matches;
	vector<DMatch> goodMatches;
	BFMatcher matcher(type, false);
	//find the two best matches for each keypoint
	matcher.knnMatch(descriptors1, descriptors2, matches, 2); //arg4: 1 for crosscheck, 2 for ratio-test
	for (int i = 0; i < matches.size(); ++i) {
		//for ratio-test
		//take match if distance in 1st match is min. 25% smaller than distance in 2nd match
		if (matches[i][0].distance < 0.75*matches[i][1].distance) goodMatches.push_back(matches[i][0]);
	}
	matches.clear();
	return goodMatches;
}

// find equal matches, which exist in two different vectors
/*
matches				  :  matches in vector 1 - should be found in vector 2
filtermatches         :  matches in vector 2
return				  :  matches which could be found in both input-vectors
*/
void KeypointMatcher::filterMatchesByMatches(vector<DMatch>& matches, vector<DMatch>& filterMatches) {
	int cnt = 0;
	// check for each match in matches: does it exist in filterMacthes? -> erase, if don´t
	for (int i = matches.size() - 1; i > 0; --i) {
		for (int j = 0; j < filterMatches.size(); ++j) {
			// if match with points A and B exists in both vectors
			// start and destination could be switched
			if (matches[i].queryIdx == filterMatches[j].queryIdx && matches[i].trainIdx == filterMatches[j].trainIdx) cnt++;
			if (matches[i].queryIdx == filterMatches[j].trainIdx && matches[i].trainIdx == filterMatches[j].queryIdx) cnt++;
		}
		if(cnt == 0) matches.erase(matches.begin() + i);
		cnt = 0;
	}
}

// erase match if distance bigger than thresh
/*
thresh			:  max. accepted distance between two keypoints of one match
matches			:  vector with matches
return			:  matches with distance <= thresh
*/
void KeypointMatcher::thresholdFilter(int thresh, vector<DMatch>& matches) {
	for (int i = matches.size()-1; i >= 0; --i) {
		if (matches[i].distance > thresh) matches.erase(matches.begin() + i);
	}
}

// ransac filter for inlier/outlier detection
/*
keypointsObject		:  keypoints in image 1 from matches
keypointsSzene      :  keypoints in image 2 from matches
matches				:  matches which include the mentioned keypoints
return				:  matches without outliers
*/
void KeypointMatcher::ransacFilter(vector<KeyPoint>& keypointsObject, vector<KeyPoint>& keypointsScene, vector<DMatch>& matches) {

	//ransac needs more than two entries
	if (matches.size() < 3) return;

	//-- Localize the object
	vector<Point2f> obj;
	vector<Point2f> scene;
	vector<int> ransacMask;
	//separate both points of a match for findHomography
	for (int i = 0; i < matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypointsObject[matches[i].queryIdx].pt);
		scene.push_back(keypointsScene[matches[i].trainIdx].pt);
	}
	Mat H = findHomography(obj, scene, CV_RANSAC, 3.0, ransacMask);
	//filter outliers
	for (int i = matches.size() - 1; i > 0; --i) {
		if (ransacMask.at(i) == 0) matches.erase(matches.begin() + i);
	}
	return;
}

// filter matches by known homography
/*
thresh			:  max distance between keypoints of a match
keypoints1      :  keypoints in image 1 from matches
keypoints2      :  keypoints in image 2 from matches
matches			:  matches which include the mentioned keypoints
H				:  homography of the szene, uses for determining spacial position of keypoints2[i] from keypoints1[i]
return			:  matches which conform to homography + thresh
*/
float KeypointMatcher::homographyFilter(int thresh, vector<KeyPoint>& keypoints1, vector<KeyPoint>& keypoints2, vector<DMatch>& matches, Mat H) {

	//if keypoints empty -> erase all matches and return
	if (keypoints1.empty()) {
		matches.clear();
		return -1;
	}
	//if matches empty -> return
	if (matches.empty()) return -1;

	vector<Point3f> v1, v2;

	for (int i = 0; i < matches.size(); ++i) {
		v1.push_back({ keypoints1[matches[i].queryIdx].pt.x, keypoints1[matches[i].queryIdx].pt.y, 1 });
		v2.push_back({ keypoints2[matches[i].trainIdx].pt.x, keypoints2[matches[i].trainIdx].pt.y, 1 });
	}
	// determine the correct point in v2
	transform(v1, v1, H);
	
	//avg distance
	float dX, dY, dXY, dTotal = 0;
	
	// check if correct points in v1 correlate to matched points in v2 -> erase if they don`t
	for (int i = matches.size() - 1; i >= 0; --i) {
		dX = abs((v1[i].x / v1[i].z) - v2[i].x);
		dY = abs((v1[i].y / v1[i].z) - v2[i].y);
		dXY = sqrt(dX*dX + dY*dY);
		if (dXY > thresh) {
			matches.erase(matches.begin() + i);
		}
		else {
			dTotal += dXY;
		}
	}
	return dTotal;
}