//============================================================================
// Name        : main.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : calls preprocessing, keypoint detectors, descriptors and test routines
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
#include "colorcvt.h"
#include <list>
#include <vector>
#include <string>
#include <time.h>

using namespace std;


// usage: path to images in argv[1] and argv[2], path to homography in argv[3]
// main function. 
//loads images and homography, 
//calls preprocessing routines, 
//calls keypoint detectors and descriptors, 
//calls matching algorithms records measurements
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

	//create empty json file for measurement output, replace file if already exists
	fileManager.createEmptyJson("output.json");

	// load image, path in argv[1]
	cout << "load image1" << endl;
	Mat img1 = imread(argv[1], 1);
	if (!img1.data) {
		cout << "ERROR: original image1 not specified" << endl;
		cout << "Press enter to exit..." << endl;
		cin.get();
		return -1;
	}

	// load image, path in argv[2]
	cout << "load image2" << endl;
	Mat img2 = imread(argv[2], 1);
	if (!img2.data) {
		cout << "ERROR: original image2 not specified" << endl;
		cout << "Press enter to exit..." << endl;
		cin.get();
		return -1;
	}
	// load homography, path in argv[3]
	Mat H = processingChainHelper.loadHomography(argv[3]);
	cout << " > done" << endl;
	
	//imshow("original", img);
	//colorcvt color;
	//color.run(img, "luv");


	// building vectors for preprocessing images
	vector<Mat> iVec1, iVec2;
	processingChainHelper.buildPreprocessingVector(iVec1, img1, iVec2, img2);

	// vector of all preprocessing steps including original image
	vector<string> pVec = controller.getPreProcessors();

	// #################### detection ####################
	// building a vector of keypoints for each image in iList
	vector<vector<KeyPoint>> kVecSift1, kVecSurf1, kVecBrisk1, kVecFreak1, kVecOrb1;
	vector<vector<KeyPoint>> kVecSift2, kVecSurf2, kVecBrisk2, kVecFreak2, kVecOrb2;
	vector<vector<vector<KeyPoint>>> helperKeypoints{kVecSift1, kVecSift2, kVecSurf1, kVecSurf2, kVecBrisk1, kVecBrisk2, kVecFreak1, kVecFreak2, kVecOrb1, kVecOrb2};
	DetectionHelper detectionHelper;
	detectionHelper.runDetection(pVec, iVec1, iVec2, kVecSift1, kVecSift2, kVecSurf1, kVecSurf2, kVecBrisk1, kVecBrisk2, kVecFreak1, kVecFreak2, kVecOrb1, kVecOrb2);


	// #################### description ####################
	// building a vector of descriptors for each image in iList1
	vector<Mat> dVecSift1, dVecSurf1, dVecBrisk1, dVecFreak1, dVecOrb1;
	vector<Mat> dVecSift2, dVecSurf2, dVecBrisk2, dVecFreak2, dVecOrb2;
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

	// #################### matching ####################
	// #################### filter by ratio ####################
	// building a vector of matches for each image-pair
	vector<vector<DMatch>> matchVecSift, matchVecSurf, matchVecBrisk, matchVecFreak, matchVecOrb;
	vector<clock_t> tRatioSift, tRatioSurf, tRatioBrisk, tRatioFreak, tRatioOrb;

	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine matches for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			time = clock();
			matchVecSift.push_back(keypointMatcher.ratioMatcher(NORM_L2, dVecSift1[i], dVecSift2[i]));
			tRatioSift.push_back(clock() - time);
		}
		if (controller.useSurf() == true) {
			time = clock();
			matchVecSurf.push_back(keypointMatcher.ratioMatcher(NORM_L2, dVecSurf1[i], dVecSurf2[i]));
			tRatioSurf.push_back(clock() - time);
		}
		if (controller.useBrisk() == true) {
			time = clock();
			matchVecBrisk.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, dVecBrisk1[i], dVecBrisk2[i]));
			tRatioBrisk.push_back(clock() - time);
		}
		if (controller.useFreak() == true) {
			time = clock();
			matchVecFreak.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, dVecFreak1[i], dVecFreak2[i]));
			tRatioFreak.push_back(clock() - time);
		}
		if (controller.useOrb() == true) {
			time = clock();
			matchVecOrb.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, dVecOrb1[i], dVecOrb2[i]));
			tRatioOrb.push_back(clock() - time);
		}
	}
	cout << " > done" << endl;


	// #################### filter by crosscheck ####################
	vector<vector<DMatch>> crossVecSift, crossVecSurf, crossVecBrisk, crossVecFreak, crossVecOrb;
	vector<clock_t> tCrossSift, tCrossSurf, tCrossBrisk, tCrossFreak, tCrossOrb;

	for (int i = 0; i < pVec.size(); ++i) {
		cout << "crosscheck for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			time = clock();
			crossVecSift.push_back(keypointMatcher.crosscheckMatcher(NORM_L2, dVecSift1[i], dVecSift2[i]));
			tCrossSift.push_back(clock() - time);
		}
		if (controller.useSurf() == true) {
			time = clock();
			crossVecSurf.push_back(keypointMatcher.crosscheckMatcher(NORM_L2, dVecSurf1[i], dVecSurf2[i]));
			tCrossSurf.push_back(clock() - time);
		}
		if (controller.useBrisk() == true) {
			time = clock();
			crossVecBrisk.push_back(keypointMatcher.crosscheckMatcher(NORM_HAMMING, dVecBrisk1[i], dVecBrisk2[i]));
			tCrossBrisk.push_back(clock() - time);
		}
		if (controller.useFreak() == true) {
			time = clock();
			crossVecFreak.push_back(keypointMatcher.crosscheckMatcher(NORM_HAMMING, dVecFreak1[i], dVecFreak2[i]));
			tCrossFreak.push_back(clock() - time);
		}
		if (controller.useOrb() == true) {
			time = clock();
			crossVecOrb.push_back(keypointMatcher.crosscheckMatcher(NORM_HAMMING, dVecOrb1[i], dVecOrb2[i]));
			tCrossOrb.push_back(clock() - time);
		}
	}
	cout << " > done" << endl;

	// #################### filter crosscheck-matches from ratio-matches ####################
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter ratio-matches for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) keypointMatcher.filterMatchesByMatches(matchVecSift[i], crossVecSift[i]);
		if (controller.useSurf() == true) keypointMatcher.filterMatchesByMatches(matchVecSurf[i], crossVecSurf[i]);
		if (controller.useBrisk() == true) keypointMatcher.filterMatchesByMatches(matchVecBrisk[i], crossVecBrisk[i]);
		if (controller.useFreak() == true) keypointMatcher.filterMatchesByMatches(matchVecFreak[i], crossVecFreak[i]);
		if (controller.useOrb() == true) keypointMatcher.filterMatchesByMatches(matchVecOrb[i], crossVecOrb[i]);
	}
	cout << " > done" << endl;

	// write crosscheckMatchTimes to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write crosscheckMatchTimes to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeTimeToJson(pVec[i] + "_sift_matchTime", "crosscheckMatchTimer", tCrossSift[i]);
		if (controller.useSurf() == true) fileManager.writeTimeToJson(pVec[i] + "_surf_matchTime", "crosscheckMatchTimer", tCrossSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeTimeToJson(pVec[i] + "_brisk_matchTime", "crosscheckMatchTimer", tCrossBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeTimeToJson(pVec[i] + "_freak_matchTime", "crosscheckMatchTimer", tCrossFreak[i]);
		if (controller.useOrb() == true) fileManager.writeTimeToJson(pVec[i] + "_orb_matchTime", "crosscheckMatchTimer", tCrossOrb[i]);
	}
	cout << " > done" << endl;

	// write ratioMatchTimes to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write ratioMatchTimes to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true && !tRatioSift.empty()) fileManager.writeTimeToJson(pVec[i] + "_sift_matchTime", "ratioMatchTimer", tRatioSift[i]);
		if (controller.useSurf() == true && !tRatioSurf.empty()) fileManager.writeTimeToJson(pVec[i] + "_surf_matchTime", "ratioMatchTimer", tRatioSurf[i]);
		if (controller.useBrisk() == true && !tRatioBrisk.empty()) fileManager.writeTimeToJson(pVec[i] + "_brisk_matchTime", "ratioMatchTimer", tRatioBrisk[i]);
		if (controller.useFreak() == true && !tRatioFreak.empty()) fileManager.writeTimeToJson(pVec[i] + "_freak_matchTime", "ratioMatchTimer", tRatioFreak[i]);
		if (controller.useOrb() == true && !tRatioOrb.empty()) fileManager.writeTimeToJson(pVec[i] + "_orb_matchTime", "ratioMatchTimer", tRatioOrb[i]);
	}
	cout << " > done" << endl;

	// write number of crosscheckMatches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write crosscheckMatches to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "crosscheckMatches", crossVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_matches", "crosscheckMatches", crossVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "crosscheckMatches", crossVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "crosscheckMatches", crossVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "crosscheckMatches", crossVecOrb[i]);
	}
	cout << " > done" << endl;

	// write number of ratioMatches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write ratioMatches to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "ratioMatches", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_matches", "ratioMatches", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "ratioMatches", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "ratioMatches", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "ratioMatches", matchVecOrb[i]);
	}
	cout << " > done" << endl;

	//// #################### filter by threshold ####################
	/// THIS FILTER IS NOT USED DUE TO EXPANTABILITY IN ADVANCE TO PREVIOUS FILTERS
	//// filter matches by threshold
	//for (int i = 0; i < pVec.size(); ++i) {
	//	cout << "filter matches by threshold for " << pVec[i] << " ..." << endl;
	//	//cout << "thresh: " << controller.getThreshold() << endl;
	//	if (controller.useSift() == true) keypointMatcher.thresholdFilter(controller.getMatchThreshold(), matchVecSift[i]);
	//	if (controller.useSurf() == true) keypointMatcher.thresholdFilter(controller.getMatchThreshold(), matchVecSurf[i]);
	//	if (controller.useBrisk() == true) keypointMatcher.thresholdFilter(controller.getMatchThreshold(), matchVecBrisk[i]);
	//	if (controller.useFreak() == true) keypointMatcher.thresholdFilter(controller.getMatchThreshold(), matchVecFreak[i]);
	//	if (controller.useOrb() == true) keypointMatcher.thresholdFilter(controller.getMatchThreshold(), matchVecOrb[i]);
	//}
	//cout << " > done" << endl;

	//// write number of filtered matches to json
	//for (int i = 0; i < pVec.size(); ++i) {
	//	cout << "write filteredMatches to json " << pVec[i] << " ..." << endl;
	//	if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "thresholdMatches", matchVecSift[i]);
	//	if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_matches", "thresholdMatches", matchVecSurf[i]);
	//	if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "thresholdMatches", matchVecBrisk[i]);
	//	if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "thresholdMatches", matchVecFreak[i]);
	//	if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "thresholdMatches", matchVecOrb[i]);
	//}
	//cout << " > done" << endl;

	// #################### filter by ransac ####################
	// filter matches by ransac
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by ransac for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) keypointMatcher.ransacFilter(kVecSift1[i], kVecSift2[i], matchVecSift[i]);
		if (controller.useSurf() == true) keypointMatcher.ransacFilter(kVecSurf1[i], kVecSurf2[i], matchVecSurf[i]);
		if (controller.useBrisk() == true) keypointMatcher.ransacFilter(kVecBrisk1[i], kVecBrisk2[i], matchVecBrisk[i]);
		if (controller.useFreak() == true) keypointMatcher.ransacFilter(kVecFreak1[i], kVecFreak2[i], matchVecFreak[i]);
		if (controller.useOrb() == true) keypointMatcher.ransacFilter(kVecOrb1[i], kVecOrb2[i], matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// write number of matches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write ransacInliers to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "ransacInliers", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_matches", "ransacInliers", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "ransacInliers", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "ransacInliers", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "ransacInliers", matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// #################### filter by real matches ####################
	// filter matches by known homography
	vector<float> avgDistSift, avgDistSurf, avgDistBrisk, avgDistFreak, avgDistOrb;
	float totalDistSift = 0, totalDistSurf = 0, totalDistBrisk = 0, totalDistFreak = 0, totalDistOrb = 0;
	int totalMatchesSift = 0, totalMatchesSurf = 0, totalMatchesBrisk = 0, totalMatchesFreak = 0, totalMatchesOrb = 0;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by known homography for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			avgDistSift.push_back(keypointMatcher.homographyFilter(controller.getHomographyThreshold(), kVecSift1[i], kVecSift2[i], matchVecSift[i], H));
			totalDistSift += avgDistSift.back();
			totalMatchesSift += matchVecSift[i].size();
			if (matchVecSift[i].size() > 0) avgDistSift.back() = avgDistSift.back() / matchVecSift[i].size();
		}
		if (controller.useSurf() == true) {
			avgDistSurf.push_back(keypointMatcher.homographyFilter(controller.getHomographyThreshold(), kVecSurf1[i], kVecSurf2[i], matchVecSurf[i], H));
			totalDistSurf += avgDistSurf.back();
			totalMatchesSurf += matchVecSurf[i].size();
			if (matchVecSurf[i].size() > 0) avgDistSurf.back() = avgDistSurf.back() / matchVecSurf[i].size();
		}
		if (controller.useBrisk() == true) {
			avgDistBrisk.push_back(keypointMatcher.homographyFilter(controller.getHomographyThreshold(), kVecBrisk1[i], kVecBrisk2[i], matchVecBrisk[i], H));
			totalDistBrisk += avgDistBrisk.back();
			totalMatchesBrisk += matchVecBrisk[i].size();
			if (matchVecBrisk[i].size() > 0) avgDistBrisk.back() = avgDistBrisk.back() / matchVecBrisk[i].size();
		}
		if (controller.useFreak() == true) {
			avgDistFreak.push_back(keypointMatcher.homographyFilter(controller.getHomographyThreshold(), kVecFreak1[i], kVecFreak2[i], matchVecFreak[i], H));
			totalDistFreak += avgDistFreak.back();
			totalMatchesFreak += matchVecFreak[i].size();
			if (matchVecFreak[i].size() > 0) avgDistFreak.back() = avgDistFreak.back() / matchVecFreak[i].size();
		}
		if (controller.useOrb() == true) {
			avgDistOrb.push_back(keypointMatcher.homographyFilter(controller.getHomographyThreshold(), kVecOrb1[i], kVecOrb2[i], matchVecOrb[i], H));
			totalDistOrb += avgDistOrb.back();
			totalMatchesOrb += matchVecOrb[i].size();
			if (matchVecOrb[i].size() > 0) avgDistOrb.back() = avgDistOrb.back() / matchVecOrb[i].size();
		}
	}
	 
	// calculate distance for all values of one detector
	if (totalMatchesSift > 0) totalDistSift = totalDistSift / totalMatchesSift;
	if (totalMatchesSurf > 0) totalDistSurf = totalDistSurf / totalMatchesSurf;
	if (totalMatchesBrisk > 0) totalDistBrisk = totalDistBrisk / totalMatchesBrisk;
	if (totalMatchesFreak > 0) totalDistFreak = totalDistFreak / totalMatchesFreak;
	if (totalMatchesOrb > 0) totalDistOrb = totalDistOrb / totalMatchesOrb;
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
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write avgDistHomography to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeDistanceToJson(pVec[i] + "_sift_avgDistHom", "avgDistHomography", avgDistSift[i]);
		if (controller.useSurf() == true) fileManager.writeDistanceToJson(pVec[i] + "_surf_avgDistHom", "avgDistHomography", avgDistSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeDistanceToJson(pVec[i] + "_brisk_avgDistHom", "avgDistHomography", avgDistBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeDistanceToJson(pVec[i] + "_freak_avgDistHom", "avgDistHomography", avgDistFreak[i]);
		if (controller.useOrb() == true) fileManager.writeDistanceToJson(pVec[i] + "_orb_avgDistHom", "avgDistHomography", avgDistOrb[i]);
	}
	cout << " > done" << endl;

	// write totalDistHomography to json
	cout << "write totalDistHomography to json " << " ..." << endl;
	if (controller.useSift() == true) fileManager.writeDistanceToJson("sift_totalDistHom", "totalDistHomography", totalDistSift);
	if (controller.useSurf() == true) fileManager.writeDistanceToJson("surf_totalDistHom", "totalDistHomography", totalDistSurf);
	if (controller.useBrisk() == true) fileManager.writeDistanceToJson("brisk_totalDistHom", "totalDistHomography", totalDistBrisk);
	if (controller.useFreak() == true) fileManager.writeDistanceToJson("freak_totalDistHom", "totalDistHomography", totalDistFreak);
	if (controller.useOrb() == true) fileManager.writeDistanceToJson("orb_totalDistHom", "totalDistHomography", totalDistOrb);
	cout << " > done" << endl;

	/*
	imshow("blurred", img1);
	Mat deblurred = img1.clone();
	deblurred = preProcessing.deblur(img1);
	imshow("deblurred", deblurred);
	waitKey(0);
	*/

	//Test test;
	//test.test(img1, img2, "Good Matches & Object detection");


	imshow("img1", img1);
	waitKey(0);

	return 0;
}


