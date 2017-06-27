//============================================================================
// Name        : KeypointDescription.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for KeypointDescription steps
//============================================================================

#include <iostream>
#include <list>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class KeypointDescription {

public:
	// constructor
	KeypointDescription(void) {};
	// destructor
	~KeypointDescription(void) {};
	//Mat surf(Mat& img, vector<KeyPoint>);
	//void sift(Mat& img, Mat& img2);
	//void orb(Mat& img1, Mat& img2, Mat& H);
	//void brisk(Mat& img1, Mat& img2);
	//void freak(Mat& img1, Mat& img2);
	//vector<KeyPoint> freak(Mat img, const char* imageName, bool showImage, bool writeFile);

	//vector<DMatch> sift1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	//vector<DMatch> surf1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	//vector<DMatch> orb1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	//vector<DMatch> brisk1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	//vector<DMatch> freak1(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);

	vector<Mat> sift(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	vector<Mat> surf(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	vector<Mat> orb(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	vector<Mat> brisk(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);
	vector<Mat> freak(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2);

	Mat sift(Mat& img, vector<KeyPoint> kp);
	Mat surf(Mat& img, vector<KeyPoint> kp);
	Mat orb(Mat& img, vector<KeyPoint> kp);
	Mat brisk(Mat& img, vector<KeyPoint> kp);
	Mat freak(Mat& img, vector<KeyPoint> kp);



	void ransacFilter(vector<KeyPoint>& keypointsObject, vector<KeyPoint>& keypointsScene, vector<DMatch>& matches);
	//vector<DMatch>ransacFilter(vector<Point2f>& obj, vector<Point2f>& scene, vector<DMatch>& vec);
	

private:
	void showMatches(Mat& img1, vector<KeyPoint> kp1, Mat& img2, vector<KeyPoint> kp2, vector<DMatch> goodMatches, string winname);

};
