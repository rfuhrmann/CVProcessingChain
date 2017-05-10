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

bool original = true; //the original image
bool histEqual = true; //histogram equalization, global contrast enhancement
bool clahe = true; //clahe filter, lokal contrast enhancement
bool nlm = true; //non local mean filter, blurring
bool bilateral = false; //bilateral filter, blurring

bool surf = true; //Speeded Up Robust Features
bool mser = true; //Maximally Stable Extremal Regions
bool brisk = false; //Binary Robust Invariant Scalable Keypoints
bool freak = false; //Fast REtinA Keypoint

Controller::Controller()
{
	if (original) preProcessors.push_back("original");
	if (histEqual) preProcessors.push_back("histEqual");
	if (clahe) preProcessors.push_back("clahe");
	if (nlm) preProcessors.push_back("nlm");
	if (bilateral) preProcessors.push_back("bilateral");

	if (surf) keypointDetectors.push_back("surf");
	if (mser) keypointDetectors.push_back("mser");
	if (brisk) keypointDetectors.push_back("brisk");
	if (freak) keypointDetectors.push_back("freak");
	
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