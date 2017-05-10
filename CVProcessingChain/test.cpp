// http://docs.opencv.org/2.4/doc/tutorials/features2d/feature_homography/feature_homography.html

#include "Test.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
//#include "opencv2/calib3d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

/** @function main */
void Test::test(Mat& img1, Mat& img2, string wName)
{
	cout << "...starting " << wName << endl;

	Mat img_object = img1.clone();
	Mat img_scene = img2.clone();
	img_object.convertTo(img_object, CV_8UC1);
	img_scene.convertTo(img_scene, CV_8UC1);

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	//SurfFeatureDetector detector(minHessian);
	Ptr<SURF> detector = SURF::create(minHessian);
	vector<KeyPoint> keypoints_object, keypoints_scene;

	detector->detect(img_object, keypoints_object);
	detector->detect(img_scene, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	Ptr<SURF> extractor = SURF::create();
	Mat descriptors_object, descriptors_scene;

	extractor->compute(img_object, keypoints_object, descriptors_object);
	extractor->compute(img_scene, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher; //Fast Library for Approximate Nearest Neighbors
	//BFMatcher matcher; //brute-force descriptor matcher
	vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//printf("-- Max dist : %f \n", max_dist);
	//printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	if (good_matches.size() < 1) {
		cout << "##no good matches"<< endl;
		cout << "##return..." << endl;
		return; 
	}
	else {
		cout << wName+"_matches: " << matches.size() << endl;
		cout << wName+"_good_matches: "<< good_matches.size() << endl;
	}

	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1));
	//,	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS
	/*
	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	//-- position for drawing corner lines
	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

	*/
	//-- Show detected matches
	//"Good Matches & Object detection"
	imshow("test_surf_"+wName, img_matches);
	
	//waitKey(0);
	return;
}