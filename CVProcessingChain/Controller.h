//============================================================================
// Name        : Controller.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for CVProcessingChain Controller
//============================================================================

#include <iostream>
#include<list>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Controller
{
public:
	Controller();
	~Controller();

	vector<string> getPreProcessors(void);
	bool useOriginal(void); 
	bool useRGB(void);
	bool useYCrCb(void);
	bool useHistEqual(void);
	bool useClahe(void);
	bool useNlm(void);
	bool useBilateral(void);

	vector<string> getKeypointDetectors(void);
	bool useSift(void);
	bool useSurf(void);
	bool useMser(void);
	bool useBrisk(void);
	bool useFreak(void);
	bool useOrb(void);

	int getThreshold();

private:

	//bool sift; //Scale-invariant feature transform
	//bool surf; //Speeded Up Robust Features
	//bool mser; //Maximally Stable Extremal Regions
	//bool brisk; //Binary Robust Invariant Scalable Keypoints
	//bool freak; //Fast REtinA Keypoint - Descriptor only, uses fast detector
	//bool orb; //Oriented FAST and Rotated BRIEF

	//int threshold;
};

