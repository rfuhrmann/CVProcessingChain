#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class DescriptionHelper
{
public:
	DescriptionHelper();
	~DescriptionHelper();

	void runDescription(
		vector<string> &pVec,
		vector<Mat> &iVec1,
		vector<Mat> &iVec2,
		vector<vector<vector<KeyPoint>>> &helperKeypoints,
		vector<vector<Mat>> &helperDescriptors);
		

	//void runDescription(
	//	vector<string> &pVec,
	//	vector<Mat> &iVec1,
	//	vector<Mat> &iVec2,
	//	vector<Mat> &descVecSift1,
	//	vector<Mat> &descVecSift2,
	//	vector<Mat> &descVecSurf1,
	//	vector<Mat> &descVecSurf2,
	//	vector<Mat> &descVecMser1,
	//	vector<Mat> &descVecMser2,
	//	vector<Mat> &descVecBrisk1,
	//	vector<Mat> &descVecBrisk2,
	//	vector<Mat> &descVecFreak1,
	//	vector<Mat> &descVecFreak2,
	//	vector<Mat> &descVecOrb1,
	//	vector<Mat> &descVecOrb2,
	//	vector<vector<KeyPoint>> &kVecSift1,
	//	vector<vector<KeyPoint>> &kVecSift2,
	//	vector<vector<KeyPoint>> &kVecSurf1,
	//	vector<vector<KeyPoint>> &kVecSurf2,
	//	vector<vector<KeyPoint>> &kVecMser1,
	//	vector<vector<KeyPoint>> &kVecMser2,
	//	vector<vector<KeyPoint>> &kVecBrisk1,
	//	vector<vector<KeyPoint>> &kVecBrisk2,
	//	vector<vector<KeyPoint>> &kVecFreak1,
	//	vector<vector<KeyPoint>> &kVecFreak2,
	//	vector<vector<KeyPoint>> &kVecOrb1,
	//	vector<vector<KeyPoint>> &kVecOrb2);
};

