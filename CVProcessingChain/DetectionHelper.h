#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class DetectionHelper
{
public:
	DetectionHelper();
	~DetectionHelper();

	void runDetection(
		vector<string> &pVec,
		vector<Mat> &iVec1, 
		vector<Mat> &iVec2,
		vector<vector<KeyPoint>> &kVecSift1,
		vector<vector<KeyPoint>> &kVecSift2,
		vector<vector<KeyPoint>> &kVecSurf1,
		vector<vector<KeyPoint>> &kVecSurf2,
		vector<vector<KeyPoint>> &kVecBrisk1,
		vector<vector<KeyPoint>> &kVecBrisk2,
		vector<vector<KeyPoint>> &kVecFreak1,
		vector<vector<KeyPoint>> &kVecFreak2,
		vector<vector<KeyPoint>> &kVecOrb1,
		vector<vector<KeyPoint>> &kVecOrb2);

};

