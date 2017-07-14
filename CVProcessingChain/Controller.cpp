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

bool original = false; //the original image
bool gray = true; //rgb color space for greyscale conversion
bool weightedGray = false; //rgb color space for weighted greyscale conversion
bool yCrCb = false; //YCrCb color space for greyscale conversion
bool luv = false; //Luv color space for greyscale conversion
bool histEqual = false; //histogram equalization, global contrast enhancement
bool clahe = false; //clahe filter, Contrast Limited Adaptive Histogram Equalization
bool bilateral = false; //bilateral filter, blurring
bool nlm = false; //non local mean filter, blurring

//verschiedene methoden zur grauwert umwandlung

bool sift = true; //Scale-invariant feature transform
bool surf = false; //Speeded Up Robust Features
bool mser = false; //Maximally Stable Extremal Regions
bool brisk = false; //Binary Robust Invariant Scalable Keypoints
bool freak = true; //Fast REtinA Keypoint - Descriptor only, uses fast detector
bool orb = false; //Oriented FAST and Rotated BRIEF

//threshold for max distance of a match (distance of keypoints)
int thresh = 1000;

Controller::Controller()
{
	preProcessors.clear();
	if (original) preProcessors.push_back("original");
	if (gray) preProcessors.push_back("gray");
	if (weightedGray) preProcessors.push_back("weightedGray");
	if (yCrCb) preProcessors.push_back("yCrCb");
	if (luv) preProcessors.push_back("luv");
	if (histEqual) preProcessors.push_back("histEqual");
	if (clahe) preProcessors.push_back("clahe");
	if (nlm) preProcessors.push_back("nlm");
	if (bilateral) preProcessors.push_back("bilateral");

	keypointDetectors.clear();
	if (sift) keypointDetectors.push_back("sift");
	if (surf) keypointDetectors.push_back("surf");
	if (mser) keypointDetectors.push_back("mser");
	if (brisk) keypointDetectors.push_back("brisk");
	if (freak) keypointDetectors.push_back("freak");
	if (orb) keypointDetectors.push_back("orb");
	
	keypointFiles.clear();
	for (auto itP = preProcessors.cbegin(); itP != preProcessors.cend(); ++itP) {
		for (auto itK = keypointDetectors.cbegin(); itK != keypointDetectors.cend(); ++itK) {
			keypointFiles.push_back(*itP+"_"+*itK+".dat");
			//cout << *itP + "_" + *itK + ".dat" << endl;
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

vector<string> Controller::getKeypointDetectors() {
	return keypointDetectors;
}

bool Controller::useSift() {
	return sift;
}
bool Controller::useSurf() {
	return surf;
}
bool Controller::useMser() {
	return mser;
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

int Controller::getThreshold() {
	return thresh;
}