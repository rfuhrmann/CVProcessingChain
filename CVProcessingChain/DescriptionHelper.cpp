#include <iostream>
#include "DescriptionHelper.h"
#include "KeypointDescription.h"
#include "Controller.h"


using namespace std;

DescriptionHelper::DescriptionHelper()
{
}


DescriptionHelper::~DescriptionHelper()
{
}

// calls description methods
/*

*/
void DescriptionHelper::runDescription(
	vector<string> &pVec,
	vector<Mat> &iVec1,
	vector<Mat> &iVec2,
	vector<vector<vector<KeyPoint>>> &helperKeypoints,
	vector<vector<Mat>> &helperDescriptors) {

	Controller controller;
	KeypointDescription keypointDescription;

	vector<vector<KeyPoint>> kVecSift1, kVecSurf1, kVecMser1, kVecBrisk1, kVecFreak1, kVecOrb1;
	vector<vector<KeyPoint>> kVecSift2, kVecSurf2, kVecMser2, kVecBrisk2, kVecFreak2, kVecOrb2;
	vector<Mat> dVecSift1, dVecSurf1, dVecBrisk1, dVecFreak1, dVecOrb1;
	vector<Mat> dVecSift2, dVecSurf2, dVecBrisk2, dVecFreak2, dVecOrb2;

	kVecSift1 = helperKeypoints[0];
	kVecSift2 = helperKeypoints[1];
	kVecSurf1 = helperKeypoints[2];
	kVecSurf2 = helperKeypoints[3];
	kVecBrisk1 = helperKeypoints[4];
	kVecBrisk2 = helperKeypoints[5];
	kVecFreak1 = helperKeypoints[6];
	kVecFreak2 = helperKeypoints[7];
	kVecOrb1 = helperKeypoints[8];
	kVecOrb2 = helperKeypoints[9];

	dVecSift1 = helperDescriptors[0];
	dVecSift2 = helperDescriptors[1];
	dVecSurf1 = helperDescriptors[2];
	dVecSurf2 = helperDescriptors[3];
	dVecBrisk1 = helperDescriptors[4];
	dVecBrisk2 = helperDescriptors[5];
	dVecFreak1 = helperDescriptors[6];
	dVecFreak2 = helperDescriptors[7];
	dVecOrb1 = helperDescriptors[8];
	dVecOrb2 = helperDescriptors[9];


	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine descriptors for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			dVecSift1.push_back(keypointDescription.sift(iVec1[i], kVecSift1[i]));
			dVecSift2.push_back(keypointDescription.sift(iVec2[i], kVecSift2[i]));
		}
		if (controller.useSurf() == true) {
			dVecSurf1.push_back(keypointDescription.surf(iVec1[i], kVecSurf1[i]));
			dVecSurf2.push_back(keypointDescription.surf(iVec2[i], kVecSurf2[i]));
		}
		if (controller.useBrisk() == true) {
			dVecBrisk1.push_back(keypointDescription.brisk(iVec1[i], kVecBrisk1[i]));
			dVecBrisk2.push_back(keypointDescription.brisk(iVec2[i], kVecBrisk2[i]));
		}
		if (controller.useFreak() == true) {
			dVecFreak1.push_back(keypointDescription.freak(iVec1[i], kVecFreak1[i]));
			dVecFreak2.push_back(keypointDescription.freak(iVec2[i], kVecFreak2[i]));
		}
		if (controller.useOrb() == true) {
			dVecOrb1.push_back(keypointDescription.orb(iVec1[i], kVecOrb1[i]));
			dVecOrb2.push_back(keypointDescription.orb(iVec2[i], kVecOrb2[i]));
		}
	}
	cout << " > done" << endl;
}
