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

//1. build matcher including crosscheck
//2. matches = matcher.knnMatches - ratio check
//3. threshold
// also meybe matches = sorted(matches, key = lambda x:x.distance)


vector<DMatch> KeypointMatcher::ratioMatcher(int type, Mat& descriptors1, Mat& descriptors2) {
	
	vector<vector<DMatch>> matches;
	vector<DMatch> goodMatches;

	BFMatcher matcher(type, true); //arg1: Type (flann, bf), arg2: chrosscheck
	matcher.knnMatch(descriptors1, descriptors2, matches, 1); //arg4: 1 for crosscheck, 2 for ratio-test
	for (int i = 0; i < matches.size(); ++i) {
		//for ratio-test
		//take match if distance in 1st match is min. 25% smaller than distance in 2nd match
		//if (matches[i][0].distance < 0.75*matches[i][1].distance) goodMatches.push_back(matches[i][0]); //ratio test
		
		//for crosscheck
		if (matches[i].empty() == false) {
			goodMatches.push_back(matches[i][0]);
		}
	}
	matches.clear();

	return goodMatches;
}

void KeypointMatcher::thresholdFilter(int thresh, vector<DMatch>& matches) {
	//cout << "before-thresh matches: " << matches.size() << endl;
	for (int i = matches.size()-1; i >= 0; --i) {
		
		if (matches[i].distance > thresh) matches.erase(matches.begin() + i);
	}
	//for (DMatch match : matches) {
	//	if (match.distance < thresh) matches.erase(matches.begin+match);
	//}

}

void KeypointMatcher::ransacFilter(vector<KeyPoint>& keypointsObject, vector<KeyPoint>& keypointsScene, vector<DMatch>& matches) {

	//ransac needs more than two entries
	if (matches.size() < 3) return;

	//-- Localize the object
	vector<Point2f> obj;
	vector<Point2f> scene;
	vector<int> ransacMask;
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
	return;// matches;
}

// filter matches by known homography
float KeypointMatcher::homographyFilter(vector<KeyPoint>& keypoints1, vector<KeyPoint>& keypoints2, vector<DMatch>& matches, Mat H) {

	cout << "HOMOGRAPHYFILTER" << endl;

	//if keypoints empty -> erase all matches and return
	if (keypoints1.empty()) {
		matches.clear();
		return -1;
	}
	//if matches empty -> return
	if (matches.empty()) return -1;

	int tolerance = 10;
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
		dX = abs(v1[i].x - v2[i].x);
		dY = abs(v1[i].y - v2[i].y);
		dXY = sqrt(dX*dX + dY*dY);
		cout << dXY << endl;
		if (dXY > tolerance) {
			matches.erase(matches.begin() + i);
		}
		else {
			dTotal += sqrt(dX*dX + dY*dY);
		}
	}
	return dTotal;
}