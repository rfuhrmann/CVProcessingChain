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

//Mat KeypointDescription::surf(Mat& img, vector<KeyPoint> kp) {
//	//Mat img2 = img.clone();
//	
//	img.convertTo(img, CV_8UC1);
//	
//	Ptr<SURF> extractor = SURF::create();
//	Mat descriptors;
//
//	extractor->compute(img, kp, descriptors);
//	return img;
//}

//vector<Mat> KeypointDescription::sift(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<SIFT> detector = SIFT::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//	//cout << "sift-vSize " << descriptors1.size() << endl;
//	return { descriptors1, descriptors2 };
//}
//
//vector<Mat> KeypointDescription::surf(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<SURF> detector = SURF::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//	//cout << "surf-vSize " << descriptors1.size() << endl;
//	return{ descriptors1, descriptors2 };
//}
//
//vector<Mat> KeypointDescription::brisk(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<BRISK> detector = BRISK::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//	//cout << "brisk-vSize " << descriptors1.size() << endl;
//	return{ descriptors1, descriptors2 };
//}
//
//vector<Mat> KeypointDescription::freak(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<FREAK> detector = FREAK::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//	//cout << "freak-vSize " << descriptors1.size() << endl;
//	return{ descriptors1, descriptors2 };
//}
//
//vector<Mat> KeypointDescription::orb(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<ORB> detector = ORB::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//	//cout << "orb-vSize " << descriptors1.size() << endl;
//	return{ descriptors1, descriptors2 };
//}

//###################################### single image #################################################

Mat KeypointDescription::sift(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<SIFT> detector = SIFT::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	//cout << "sift-vSize " << descriptors.size() << endl;
	return descriptors;
}

Mat KeypointDescription::surf(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<SURF> detector = SURF::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	//cout << "sift-vSize " << descriptors.size() << endl;
	return descriptors;
}

Mat KeypointDescription::brisk(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<BRISK> detector = BRISK::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	//cout << "sift-vSize " << descriptors.size() << endl;
	return descriptors;
}

Mat KeypointDescription::freak(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<FREAK> detector = FREAK::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	//cout << "sift-vSize " << descriptors.size() << endl;
	return descriptors;
}

Mat KeypointDescription::orb(Mat& img, vector<KeyPoint> kp) {
	img.convertTo(img, CV_8UC1);
	Ptr<ORB> detector = ORB::create();
	Mat descriptors;
	detector->compute(img, kp, descriptors);
	//cout << "sift-vSize " << descriptors.size() << endl;
	return descriptors;
}

//vector<DMatch> KeypointDescription::sift1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<SIFT> detector = SIFT::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//	
//	//BFMatcher matcher(NORM_L2, false);
//	vector< DMatch > matches;
//
//	KeypointMatcher kpMatcher;
//	//matches = kpMatcher.ratioMatcher(matcher, descriptors1, descriptors2);
//	matches = kpMatcher.ratioMatcher(NORM_L2, descriptors1, descriptors2);
//	//matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < matches.size(); i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < matches.size(); i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//		good_matches.push_back(matches[i]);
//		//}
//	}
//	//showMatches(img1, kp1, img2, kp2, good_matches, "###sift_matches");
//	return good_matches;
//}
//
//vector<DMatch> KeypointDescription::surf1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<SURF> detector = SURF::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//
//	//BFMatcher matcher(NORM_L2, false);
//	vector< DMatch > matches;
//
//	KeypointMatcher kpMatcher;
//	matches = kpMatcher.ratioMatcher(NORM_L2, descriptors1, descriptors2);
//	//matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < matches.size(); i++) //descriptor1.rows
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < matches.size(); i++)
//	{
//		if (matches[i].distance < 300 * min_dist) //3*min
//		{
//			good_matches.push_back(matches[i]);
//		}
//	}
//	//showMatches(img1, kp1, img2, kp2, good_matches, "###surf_matches");
//	return good_matches;
//}
//
//vector<DMatch> KeypointDescription::brisk1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<BRISK> detector = BRISK::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//
//	//BFMatcher matcher(NORM_HAMMING, false);
//	vector< DMatch > matches;
//
//	KeypointMatcher kpMatcher;
//	matches = kpMatcher.ratioMatcher(NORM_HAMMING, descriptors1, descriptors2);
//	//matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < matches.size(); i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < matches.size(); i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//		good_matches.push_back(matches[i]);
//		//}
//	}
//	//showMatches(img1, kp1, img2, kp2, good_matches, "###orb_matches");
//	return good_matches;
//}
//
//vector<DMatch> KeypointDescription::freak1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<FREAK> detector = FREAK::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//
//	//BFMatcher matcher(NORM_HAMMING, false);
//	vector< DMatch > matches;
//
//	KeypointMatcher kpMatcher;
//	matches = kpMatcher.ratioMatcher(NORM_HAMMING, descriptors1, descriptors2);
//	//matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < matches.size(); i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < matches.size(); i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//		good_matches.push_back(matches[i]);
//		//}
//	}
//	//showMatches(img1, kp1, img2, kp2, good_matches, "###orb_matches");
//	return good_matches;
//}
//
//vector<DMatch> KeypointDescription::orb1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//	Ptr<ORB> detector = ORB::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, kp1, descriptors1);
//	detector->compute(img2, kp2, descriptors2);
//
//	//BFMatcher matcher(NORM_HAMMING, false);
//	vector< DMatch > matches;
//
//	KeypointMatcher kpMatcher;
//	matches = kpMatcher.ratioMatcher(NORM_HAMMING, descriptors1, descriptors2);
//	//matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < matches.size(); i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < matches.size(); i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//		good_matches.push_back(matches[i]);
//		//}
//	}
//	//showMatches(img1, kp1, img2, kp2, good_matches, "###orb_matches");
//	return good_matches;
//}
//
//void KeypointDescription::orb(Mat& img1, Mat& img2, Mat& H) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//
//	//-- Step 1: Detect the keypoints using SURF Detector
//	Ptr<ORB> detector = ORB::create();
//	vector<KeyPoint> keypoints1, keypoints2;
//
//	detector->detect(img1, keypoints1);
//	detector->detect(img2, keypoints2);
//
//	//write keypoints to json document
//	FileManager fileManager;
//	fileManager.writeKeypointsToJson("orb_kp1", keypoints1);
//	fileManager.writeKeypointsToJson("orb_kp2", keypoints2);
//
//	//-- Step 2: Calculate descriptors (feature vectors)
//	//Ptr<SURF> extractor = SURF::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, keypoints1, descriptors1);
//	detector->compute(img2, keypoints2, descriptors2);
//
//	//-- Step 3: Matching descriptor vectors using FLANN matcher
//	//FlannBasedMatcher matcher; //Fast Library for Approximate Nearest Neighbors
//							   //BFMatcher matcher; //brute-force descriptor matcher
//	BFMatcher matcher;
//	vector< DMatch > matches;
//	matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//printf("-- Max dist : %f \n", max_dist);
//	//printf("-- Min dist : %f \n", min_dist);
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//			good_matches.push_back(matches[i]);
//		//}
//	}
//
//	//########## ALL FOR RANSAC ##########
//	ransacFilter(keypoints1, keypoints2, good_matches);
//
//	//write matches to json document
//	//FileManager fileManager;
//	//fileManager.writeMatchesToJson("orb_matches", good_matches);
//
//	//########## COMPARE TO GROUND TRUTH ##########
//	//good_matches.at(0).
//
//
//	Mat img_matches;
//	drawMatches(img1, keypoints1, img2, keypoints2,
//		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1));
//
//	imshow("test_orb_", img_matches);
//	//waitKey(0);
//}
//
//void KeypointDescription::brisk(Mat& img1, Mat& img2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//
//	//-- Step 1: Detect the keypoints using SURF Detector
//	Ptr<BRISK> detector = BRISK::create();
//	vector<KeyPoint> keypoints1, keypoints2;
//
//	detector->detect(img1, keypoints1);
//	detector->detect(img2, keypoints2);
//
//	//-- Step 2: Calculate descriptors (feature vectors)
//	//Ptr<BRISK> extractor = BRISK::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, keypoints1, descriptors1);
//	detector->compute(img2, keypoints2, descriptors2);
//
//	//-- Step 3: Matching descriptor vectors using FLANN matcher
//	//FlannBasedMatcher matcher; //Fast Library for Approximate Nearest Neighbors
//							   //BFMatcher matcher; //brute-force descriptor matcher
//	BFMatcher matcher; //(NORM_HAMMING)
//	vector< DMatch > matches;
//	matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//printf("-- Max dist : %f \n", max_dist);
//	//printf("-- Min dist : %f \n", min_dist);
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//			good_matches.push_back(matches[i]);
//		//}
//	}
//
//	//########## ALL FOR RANSAC ##########
//	//good_matches = ransacFilter(keypoints1, keypoints2, good_matches);
//
//	Mat img_matches;
//	drawMatches(img1, keypoints1, img2, keypoints2,
//		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1));
//	cout << "#matches: " << good_matches.size() << endl;
//	imshow("test_brisk_", img_matches);
//	//waitKey(0);
//}
//
//void KeypointDescription::sift(Mat& img1, Mat& img2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//
//	//-- Step 1: Detect the keypoints using SURF Detector
//	Ptr<SIFT> detector = SIFT::create();
//	vector<KeyPoint> keypoints1, keypoints2;
//
//	detector->detect(img1, keypoints1);
//	detector->detect(img2, keypoints2);
//
//	//-- Step 2: Calculate descriptors (feature vectors)
//	//Ptr<SURF> extractor = SURF::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, keypoints1, descriptors1);
//	detector->compute(img2, keypoints2, descriptors2);
//
//	//-- Step 3: Matching descriptor vectors using FLANN matcher
//	//FlannBasedMatcher matcher; //Fast Library for Approximate Nearest Neighbors
//	//BFMatcher matcher; //brute-force descriptor matcher
//	BFMatcher matcher;
//	vector< DMatch > matches;
//	matcher.match(descriptors1, descriptors2, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//printf("-- Max dist : %f \n", max_dist);
//	//printf("-- Min dist : %f \n", min_dist);
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//		good_matches.push_back(matches[i]);
//		//}
//	}
//
//	//########## ALL FOR RANSAC ##########
//	//good_matches = ransacFilter(keypoints1, keypoints2, good_matches);
//
//	Mat img_matches;
//	drawMatches(img1, keypoints1, img2, keypoints2,
//		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1));
//
//	imshow("test_sift_", img_matches);
//	//waitKey(0);
//}
//
//void KeypointDescription::freak(Mat& img1, Mat& img2) {
//	img1.convertTo(img1, CV_8UC1);
//	img2.convertTo(img2, CV_8UC1);
//
//	//-- Step 1: Detect the keypoints using SURF Detector
//	Ptr<FREAK> detector = FREAK::create();
//	vector<KeyPoint> keypointsObject, keypointsScene;
//
//	detector->detect(img1, keypointsObject);
//	detector->detect(img2, keypointsScene);
//
//	//-- Step 2: Calculate descriptors (feature vectors)
//	//Ptr<SURF> extractor = SURF::create();
//	Mat descriptors1, descriptors2;
//
//	detector->compute(img1, keypointsObject, descriptors1);
//	detector->compute(img2, keypointsScene, descriptors2);
//
//	//-- Step 3: Matching descriptor vectors using FLANN matcher
//	//FlannBasedMatcher matcher; //Fast Library for Approximate Nearest Neighbors
//	//BFMatcher matcher; //brute-force descriptor matcher
//	BFMatcher matcher;
//	vector< DMatch > matches;
//	matcher.match(descriptors1, descriptors2, matches);
//	
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//printf("-- Max dist : %f \n", max_dist);
//	//printf("-- Min dist : %f \n", min_dist);
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < descriptors1.rows; i++)
//	{
//		//if (matches[i].distance < 3 * min_dist)
//		//{
//		good_matches.push_back(matches[i]);
//		//}
//	}
//
//	//########## ALL FOR RANSAC ##########
//	//good_matches = ransacFilter(keypoints1, keypoints2, good_matches);
//
//	Mat img_matches;
//	drawMatches(img1, keypointsObject, img2, keypointsScene,
//		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1));
//
//	imshow("test_freak_", img_matches);
//	//waitKey(0);
//}

void KeypointDescription::ransacFilter(vector<KeyPoint>& keypointsObject, vector<KeyPoint>& keypointsScene, vector<DMatch>& matches) {

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

void KeypointDescription::showMatches(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2, vector<DMatch> goodMatches, string winname) {
	Mat imgMatches;
	drawMatches(img1, kp1, img2, kp2,
		goodMatches, imgMatches, Scalar::all(-1), Scalar::all(-1));
	imshow(winname, imgMatches);
}