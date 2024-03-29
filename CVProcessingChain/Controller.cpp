//============================================================================
// Name        : Controller.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : holds Variables for controlling the processing chain
//============================================================================

#include "Controller.h"
#include "KeypointDetection.h"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

vector<string> preProcessors;
vector<string> keypointDetectors;
list<string> keypointFiles;

// #####################################################################
// #################### START SETTINGS FOR NEXT RUN ####################
// #####################################################################
// ############# options for the next run need to be TRUE ##############
// ######### options which should be avoided need to be FALSE ##########
// #####################################################################
// ################# feel free to edit given settings ##################
// ################# multiple selections are possible ##################
// #####################################################################

bool original = false; //DO NOT USE: the original image, maybe colored
bool gray = false; //rgb color space for greyscale conversion
bool weightedGray = true; //rgb color space for weighted greyscale conversion
bool yCrCb = false; //YCrCb color space for greyscale conversion
bool luv = false; //Luv color space for greyscale conversion
bool lab = false; //Lab color space for greyscale conversion

bool histEqual = false; //histogram equalization, global contrast enhancement - input-image: gray
bool clahe = false; //contrast limited adaptive histogram equalization, lokal contrast enhancement - input-image: gray
bool bilateral = false; //bilateral filter, blurring - input-image: gray or rgb
bool nlm = false; //non local mean filter, blurring - input-image: gray or rgb
bool bm3d = false; //block matching and 3d filtering - input-image: gray

bool sift = false; //Scale-Invariant Feature Transform
bool surf = true; //Speeded Up Robust Features
bool brisk = false; //Binary Robust Invariant Scalable Keypoints
bool freak = false; //Fast REtinA Keypoint - Descriptor only, uses FAST detector
bool orb = false; //Oriented fast and Rotated Brief

int threshMatches = 1000; //NOT USED due to purposelessness - threshold for max distance of a match for thresholdFilter (distance of keypoints)
int threshHomography = 9; //threshold for max distance of a match for homographyFilter (distance of deskriptor points)

// ###################################################################
// #################### END SETTINGS FOR NEXT RUN ####################
// ###################################################################
// ####### the following parameters will be set automatically, #######
// ################ depending on your previous choices ###############
// ###################################################################

Controller::Controller()
{
	preProcessors.clear();
	if (original) preProcessors.push_back("original");
	if (gray) preProcessors.push_back("gray");
	if (weightedGray) preProcessors.push_back("weightedGray");
	if (yCrCb) preProcessors.push_back("yCrCb");
	if (luv) preProcessors.push_back("luv");
	if (lab) preProcessors.push_back("lab");

	if (histEqual) preProcessors.push_back("histEqual");
	if (clahe) preProcessors.push_back("clahe");
	if (nlm) preProcessors.push_back("nlm");
	if (bilateral) preProcessors.push_back("bilateral");
	if (bm3d) preProcessors.push_back("bm3d");

	keypointDetectors.clear();
	if (sift) keypointDetectors.push_back("sift");
	if (surf) keypointDetectors.push_back("surf");
	if (brisk) keypointDetectors.push_back("brisk");
	if (freak) keypointDetectors.push_back("freak");
	if (orb) keypointDetectors.push_back("orb");
	
	keypointFiles.clear();
	for (auto itP = preProcessors.cbegin(); itP != preProcessors.cend(); ++itP) {
		for (auto itK = keypointDetectors.cbegin(); itK != keypointDetectors.cend(); ++itK) {
			keypointFiles.push_back(*itP+"_"+*itK+".dat");
		}
	}
}

Controller::~Controller(){}

vector<string> Controller::getPreProcessors() {
	return preProcessors;
}

bool Controller::useOriginal() {
	return original;
}
bool Controller::useGray() {
	return gray;
}
bool Controller::useWeightedGray() {
	return weightedGray;
}
bool Controller::useYCrCb() {
	return yCrCb;
}
bool Controller::useLuv() {
	return luv;
}
bool Controller::useLab() {
	return lab;
}
bool Controller::useHistEqual() {
	return histEqual;
}
bool Controller::useClahe() {
	return clahe;
}
bool Controller::useNlm() {
	return nlm;
}
bool Controller::useBilateral() {
	return bilateral;
}
bool Controller::useBM3D () {
	return bm3d;
}

vector<string> Controller::getKeypointDetectors() {
	return keypointDetectors;
}

bool Controller::useSift() {
	return sift;
}
bool Controller::useSurf() {
	return surf;
}
bool Controller::useBrisk() {
	return brisk;
}
bool Controller::useFreak() {
	return freak;
}
bool Controller::useOrb() {
	return orb;
}

int Controller::getMatchThreshold() {
	return threshMatches;
}
int Controller::getHomographyThreshold() {
	return threshHomography;
}