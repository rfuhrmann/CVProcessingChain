//============================================================================
// Name        : main.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : calls preprocessing, keypoint detectors and test routines
//============================================================================

#include <iostream>

#include "Controller.h"
#include "PreProcessing.h"
#include "DetectionHelper.h"
#include "DescriptionHelper.h"
#include "KeypointDetection.h"
#include "KeypointDescription.h"
#include "FileManager.h"
#include "KeypointMatcher.h"
#include "ProcessingChainHelper.h"
#include "Test.h"
#include <list>
#include <vector>
#include <string>
#include <time.h>

using namespace std;


// usage: path to image in argv[1]
// main function. loads image, calls preprocessing routines, calls keypoint detectors, records processing times
int main(int argc, char** argv) {

	// check if enough arguments are defined
	if (argc < 3) {
		cout << "Usage:\n\tparameter1: path to imgage1" << endl;
		cout << "Usage:\n\tparameter2: path to imgage2" << endl;
		cout << "Usage:\n\tparameter3: path to homography" << endl;
		cout << "\nPress enter to exit" << endl;
		cin.get();
		return -1;
	}

	// construct objects
	Controller controller;
	PreProcessing preProcessing;
	KeypointDetection keypointDetection;
	KeypointDescription keypointDescription;
	FileManager fileManager;
	KeypointMatcher keypointMatcher;
	ProcessingChainHelper processingChainHelper;
	fileManager.createEmptyJson("output.json");

	// load image, path in argv[1]
	cout << "load image1" << endl;
	Mat img1 = imread(argv[1], 1);// CV_LOAD_IMAGE_ANYCOLOR); //CV_LOAD_IMAGE_ANYDEPTH | 0);
	if (!img1.data) {
		cout << "ERROR: original image1 not specified" << endl;
		cout << "Press enter to exit..." << endl;
		cin.get();
		return -1;
	}
	Mat img = img1.clone();
	//cvtColor(img, img1, CV_BGR2GRAY);
	// load image, path in argv[2]
	cout << "load image2" << endl;
	Mat img2 = imread(argv[2], 1);// CV_LOAD_IMAGE_ANYCOLOR); //CV_LOAD_IMAGE_ANYDEPTH | 0);
	if (!img2.data) {
		cout << "ERROR: original image2 not specified" << endl;
		cout << "Press enter to exit..." << endl;
		cin.get();
		return -1;
	}
	// load homography, path in argv[3]
	Mat H = processingChainHelper.loadHomography(argv[3]);
	
    // convert U8 to 32F
    //img1.convertTo(img1, CV_32FC1);
	//img2.convertTo(img2, CV_32FC1);
    cout << " > done" << endl;

	// building vectors for preprocessing images including original image
	vector<Mat> iVec1, iVec2;
	processingChainHelper.buildPreprocessingVector(iVec1, img1, iVec2, img2);

	// vector of all preprocessing steps including original image
	vector<string> pVec = controller.getPreProcessors();


	// #################### detection ####################
	// building a vector of keypoints for each image in iList
	vector<vector<KeyPoint>> kVecSift1, kVecSurf1, kVecMser1, kVecBrisk1, kVecFreak1, kVecOrb1;
	vector<vector<KeyPoint>> kVecSift2, kVecSurf2, kVecMser2, kVecBrisk2, kVecFreak2, kVecOrb2;
	vector<vector<vector<KeyPoint>>> helperKeypoints{kVecSift1, kVecSift2, kVecSurf1, kVecSurf2, kVecBrisk1, kVecBrisk2, kVecFreak1, kVecFreak2, kVecOrb1, kVecOrb2};
	DetectionHelper detectionHelper;
	detectionHelper.runDetection(pVec, iVec1, iVec2, kVecSift1, kVecSift2, kVecSurf1, kVecSurf2, kVecMser1, kVecMser2, kVecBrisk1, kVecBrisk2, kVecFreak1, kVecFreak2, kVecOrb1, kVecOrb2);


	// #################### description ####################
	// building a vector of descriptors for each image in iList1
	//vector<vector<Mat>> descVecSift, descVecSurf, descVecBrisk, descVecFreak, descVecOrb;
	vector<Mat> dVecSift1, dVecSurf1, dVecBrisk1, dVecFreak1, dVecOrb1;
	vector<Mat> dVecSift2, dVecSurf2, dVecBrisk2, dVecFreak2, dVecOrb2;
	//vector<vector<Mat>> helperDescriptors{ dVecSift1, dVecSift2, dVecSurf1, dVecSurf2, dVecBrisk1, dVecBrisk2, dVecFreak1, dVecFreak2, dVecOrb1, dVecOrb2 };
	//DescriptionHelper descriptionHelper;
	//descriptionHelper.runDescription(pVec, iVec1, iVec2, helperKeypoints, helperDescriptors);
	clock_t time;
	vector<clock_t> tDSift1, tDSift2, tDSurf1, tDSurf2, tDBrisk1, tDBrisk2, tDFreak1, tDFreak2, tDOrb1, tDOrb2;

	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine descriptors for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			time = clock();
			dVecSift1.push_back(keypointDescription.sift(iVec1[i], kVecSift1[i]));
			tDSift1.push_back(clock() - time);
			time = clock();
			dVecSift2.push_back(keypointDescription.sift(iVec2[i], kVecSift2[i]));
			tDSift2.push_back(clock() - time);
		}
		if (controller.useSurf() == true) {
			time = clock();
			dVecSurf1.push_back(keypointDescription.surf(iVec1[i], kVecSurf1[i]));
			tDSurf1.push_back(clock() - time);
			time = clock();
			dVecSurf2.push_back(keypointDescription.surf(iVec2[i], kVecSurf2[i]));
			tDSurf2.push_back(clock() - time);
		}
		if (controller.useBrisk() == true) {
			time = clock();
			dVecBrisk1.push_back(keypointDescription.brisk(iVec1[i], kVecBrisk1[i]));
			tDBrisk1.push_back(clock() - time);
			time = clock();
			dVecBrisk2.push_back(keypointDescription.brisk(iVec2[i], kVecBrisk2[i]));
			tDBrisk2.push_back(clock() - time);
		}
		if (controller.useFreak() == true) {
			time = clock();
			dVecFreak1.push_back(keypointDescription.freak(iVec1[i], kVecFreak1[i]));
			tDFreak1.push_back(clock() - time);
			time = clock();
			dVecFreak2.push_back(keypointDescription.freak(iVec2[i], kVecFreak2[i]));
			tDFreak2.push_back(clock() - time);
		}
		if (controller.useOrb() == true) {
			time = clock();
			dVecOrb1.push_back(keypointDescription.orb(iVec1[i], kVecOrb1[i]));
			tDOrb1.push_back(clock() - time);
			time = clock();
			dVecOrb2.push_back(keypointDescription.orb(iVec2[i], kVecOrb2[i]));
			tDOrb2.push_back(clock() - time);
		}
	}
	cout << " > done" << endl;

	// write descriptionTimes to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write descriptionTimes to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			fileManager.writeTimeToJson(pVec[i] + "_sift1_descriptionTime", "descriptorTimer", tDSift1[i]);
			fileManager.writeTimeToJson(pVec[i] + "_sift2_descriptionTime", "descriptorTimer", tDSift2[i]);
		}
		if (controller.useSurf() == true) {
			fileManager.writeTimeToJson(pVec[i] + "_surf1_descriptionTime", "descriptorTimer", tDSurf1[i]);
			fileManager.writeTimeToJson(pVec[i] + "_surf2_descriptionTime", "descriptorTimer", tDSurf1[i]);
		}
		if (controller.useBrisk() == true) {
			fileManager.writeTimeToJson(pVec[i] + "_brisk1_descriptionTime", "descriptorTimer", tDBrisk1[i]);
			fileManager.writeTimeToJson(pVec[i] + "_brisk2_descriptionTime", "descriptorTimer", tDBrisk2[i]);
		}
		if (controller.useFreak() == true) {
			fileManager.writeTimeToJson(pVec[i] + "_freak1_descriptionTime", "descriptorTimer", tDFreak1[i]);
			fileManager.writeTimeToJson(pVec[i] + "_freak2_descriptionTime", "descriptorTimer", tDFreak2[i]);
		}
		if (controller.useOrb() == true) {
			fileManager.writeTimeToJson(pVec[i] + "_orb1_descriptionTime", "descriptorTimer", tDOrb1[i]);
			fileManager.writeTimeToJson(pVec[i] + "_orb2_descriptionTime", "descriptorTimer", tDOrb2[i]);
		}
	}
	cout << " > done" << endl;

	// #################### match ####################
	// #################### filter by crosscheck ####################
	// #################### filter by ratio ####################
	// building a vector of matches for each image-pair
	vector<vector<DMatch>> matchVecSift, matchVecSurf, matchVecBrisk, matchVecFreak, matchVecOrb;
	vector<clock_t> tMSift, tMSurf, tMBrisk, tMFreak, tMOrb;

	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine matches for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			time = clock();
			matchVecSift.push_back(keypointMatcher.ratioMatcher(NORM_L2, dVecSift1[i], dVecSift2[i]));
			tMSift.push_back(clock() - time);
		}
		if (controller.useSurf() == true) {
			time = clock();
			matchVecSurf.push_back(keypointMatcher.ratioMatcher(NORM_L2, dVecSurf1[i], dVecSurf2[i]));
			tMSurf.push_back(clock() - time);
		}
		if (controller.useBrisk() == true) {
			time = clock();
			matchVecBrisk.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, dVecBrisk1[i], dVecBrisk2[i]));
			tMBrisk.push_back(clock() - time);
		}
		if (controller.useFreak() == true) {
			time = clock();
			matchVecFreak.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, dVecFreak1[i], dVecFreak2[i]));
			tMFreak.push_back(clock() - time);
		}
		if (controller.useOrb() == true) {
			time = clock();
			matchVecOrb.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, dVecOrb1[i], dVecOrb2[i]));
			tMOrb.push_back(clock() - time);
		}
	}
	cout << " > done" << endl;

	// write matchTimes to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write matchTimes to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeTimeToJson(pVec[i] + "_sift_matchTime", "matchTimer", tMSift[i]);
		if (controller.useSurf() == true) fileManager.writeTimeToJson(pVec[i] + "_surf_matchTime", "matchTimer", tMSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeTimeToJson(pVec[i] + "_brisk_matchTime", "matchTimer", tMBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeTimeToJson(pVec[i] + "_freak_matchTime", "matchTimer", tMFreak[i]);
		if (controller.useOrb() == true) fileManager.writeTimeToJson(pVec[i] + "_orb_matchTime", "matchTimer", tMOrb[i]);
	}
	cout << " > done" << endl;

	// write number of bfmMatches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write bfmMatches to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "bfmMatches", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_matches", "bfmMatches", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "bfmMatches", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "bfmMatches", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "bfmMatches", matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// #################### filter by threshold ####################
	// filter matches by threshold
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by threshold for " << pVec[i] << " ..." << endl;
		//cout << "thresh: " << controller.getThreshold() << endl;
		if (controller.useSift() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecSift[i]);
		if (controller.useSurf() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecSurf[i]);
		if (controller.useBrisk() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecBrisk[i]);
		if (controller.useFreak() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecFreak[i]);
		if (controller.useOrb() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// write number of filtered matches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write filteredMatches to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "filteredMatches", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_matches", "filteredMatches", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "filteredMatches", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "filteredMatches", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "filteredMatches", matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// #################### filter by ransac ####################
	// filter matches by ransac
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by ransac for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) keypointDescription.ransacFilter(kVecSift1[i], kVecSift2[i], matchVecSift[i]);
		if (controller.useSurf() == true) keypointDescription.ransacFilter(kVecSurf1[i], kVecSurf2[i], matchVecSurf[i]);
		if (controller.useBrisk() == true) keypointDescription.ransacFilter(kVecBrisk1[i], kVecBrisk2[i], matchVecBrisk[i]);
		if (controller.useFreak() == true) keypointDescription.ransacFilter(kVecFreak1[i], kVecFreak2[i], matchVecFreak[i]);
		if (controller.useOrb() == true) keypointDescription.ransacFilter(kVecOrb1[i], kVecOrb2[i], matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// write number of matches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write ransacInliers to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "ransacInliers", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i]+"_surf_matches", "ransacInliers", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "ransacInliers", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "ransacInliers", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "ransacInliers", matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// #################### filter by real matches ####################
	// filter matches by known homography
	float avgDistSift = 0, avgDistSurf = 0, avgDistBrisk = 0, avgDistFreak = 0, avgDistOrb = 0;
	int totalMatchesSift = 0, totalMatchesSurf = 0, totalMatchesBrisk = 0, totalMatchesFreak = 0, totalMatchesOrb = 0;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by known homography for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			avgDistSift += keypointMatcher.homographyFilter(kVecSift1[i], kVecSift2[i], matchVecSift[i], H);
			totalMatchesSift += matchVecSift[i].size();
		}
		if (controller.useSurf() == true) {
			avgDistSurf += keypointMatcher.homographyFilter(kVecSurf1[i], kVecSurf2[i], matchVecSurf[i], H);
			totalMatchesSurf += matchVecSurf[i].size();
		}
		if (controller.useBrisk() == true) {
			avgDistBrisk += keypointMatcher.homographyFilter(kVecBrisk1[i], kVecBrisk2[i], matchVecBrisk[i], H);
			totalMatchesBrisk += matchVecBrisk[i].size();
		}
		if (controller.useFreak() == true) {
			avgDistFreak += keypointMatcher.homographyFilter(kVecFreak1[i], kVecFreak2[i], matchVecFreak[i], H);
			totalMatchesFreak += matchVecFreak[i].size();
		}
		if (controller.useOrb() == true) {
			avgDistOrb += keypointMatcher.homographyFilter(kVecOrb1[i], kVecOrb2[i], matchVecOrb[i], H);
			totalMatchesOrb += matchVecOrb[i].size();
		}
	}
	if (totalMatchesSift > 0) avgDistSift = avgDistSift / totalMatchesSift;
	if (totalMatchesSurf > 0) avgDistSurf = avgDistSurf / totalMatchesSurf;
	if (totalMatchesBrisk > 0) avgDistBrisk = avgDistBrisk / totalMatchesBrisk;
	if (totalMatchesFreak > 0) avgDistFreak = avgDistFreak / totalMatchesFreak;
	if (totalMatchesOrb > 0) avgDistOrb = avgDistOrb / totalMatchesOrb;

	//avgDistSift = (round(avgDistSift * 10000)) / 10000;
	//avgDistSurf = (round(avgDistSurf * 10000)) / 10000;
	//avgDistBrisk = (round(avgDistBrisk * 10000)) / 10000;
	//avgDistFreak = (round(avgDistFreak * 10000)) / 10000;
	//avgDistOrb = (round(avgDistOrb * 10000)) / 10000;

	cout << " > done" << endl;

	// write realMatches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write realMatches to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_realMatches", "realMatches", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_realMatches", "realMatches", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_realMatches", "realMatches", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_realMatches", "realMatches", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_realMatches", "realMatches", matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// write avgDistHomography to json
	cout << "write avgDistHomography to json " << " ..." << endl;
	if (controller.useSift() == true) fileManager.writeDistanceToJson("sift_avgDistHom", "avgDistHomography", avgDistSift);
	if (controller.useSurf() == true) fileManager.writeDistanceToJson("surf_avgDistHom", "avgDistHomography", avgDistSurf);
	if (controller.useBrisk() == true) fileManager.writeDistanceToJson("brisk_avgDistHom", "avgDistHomography", avgDistBrisk);
	if (controller.useFreak() == true) fileManager.writeDistanceToJson("freak_avgDistHom", "avgDistHomography", avgDistFreak);
	if (controller.useOrb() == true) fileManager.writeDistanceToJson("orb_avgDistHom", "avgDistHomography", avgDistOrb);
	cout << " > done" << endl;

	//cout << "distances: " << endl << avgDistSift << endl << avgDistSurf << endl << avgDistBrisk << endl << avgDistFreak << endl << avgDistOrb << endl;
	//// write avgDistHomography to json
	//cout << "write avgDistHomography to json " << " ..." << endl;
	//if (controller.useSift() == true) fileManager.writeDistanceToJson("sift_avgDistHom", "avgDistHomography", (round(avgDistSift*10000))/10000);
	//if (controller.useSurf() == true) fileManager.writeDistanceToJson("surf_avgDistHom", "avgDistHomography", (round(avgDistSurf * 10000)) / 10000);
	//if (controller.useBrisk() == true) fileManager.writeDistanceToJson("brisk_avgDistHom", "avgDistHomography", (round(avgDistBrisk * 10000)) / 10000);
	//if (controller.useFreak() == true) fileManager.writeDistanceToJson("freak_avgDistHom", "avgDistHomography", (round(avgDistFreak * 10000)) / 10000);
	//if (controller.useOrb() == true) fileManager.writeDistanceToJson("orb_avgDistHom", "avgDistHomography", (round(avgDistOrb * 10000)) / 10000);
	//cout << " > done" << endl;


	
	//Test test;
	//test.test(img1, img2, "Good Matches & Object detection");

	
	//keypointDescription.orb(img, img2, H);
	//keypointDescription.brisk(img, img2);
	//keypointDescription.sift(img, img2);
	//keypointDescription.freak(img, img2);



	//vector<KeyPoint> keypoints;
	//fileManager.writeKeypointsToJson("detektor2", keypoints);
	
	img1 = img.clone();

	imshow("bgr", img1);
	waitKey(0);
	//cvtColor(img1, img, CV_BGR2GRAY);
	//cout << img.col(0).row(0) << endl;
	//imshow("gray", img);
	//waitKey(0);
	cvtColor(img1, img, CV_BGR2Luv);
	imshow("luv1", img);
	waitKey(0);
	//cvtColor(img, img, CV_BGR2GRAY);
	//cout << img.at<float>(0, 0);
	//for (int y = 0; y < img.rows; ++y) {
	//	for (int x = 0; x < img.cols; ++x) {
	//		//img.col(x).row(y).data[0] = 0; //blue (0, img.col(x).row(y).data[1], 0);//Point3d(img.at<Point3d>(x, y).x,0,0); img.col(x).row(y).data[0]
	//		img.col(x).row(y).data[1] = 0; //green
	//		img.col(x).row(y).data[2] = 0; //red
	//	}
	//}
	//vector<Mat> planes, planes2;
	////Mat plan[3];
	//split(img, planes);
	////split(img, plan);
	//imshow("luv", planes[0]);
	//waitKey(0);
	//cvtColor(img1, img, CV_BGR2YCrCb);
	//split(img, planes2);
	//imshow("ycbcr", planes2[0]);
	//waitKey(0);

   return 0;
} 


