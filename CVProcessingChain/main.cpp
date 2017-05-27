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
#include "KeypointDetection.h"
#include "KeypointDescription.h"
#include "Test.h"
#include<list>
#include<vector>
//#include<fstream>

using namespace std;


// usage: path to image in argv[1]
// main function. loads image, calls preprocessing routines, calls keypoint detectors, records processing times
int main(int argc, char** argv) {

	//Controller controller;
	
	//definition of used preprocessing methods
	//const string imageName[] = { "original", "histEqual" };
	//definition of used keypoint detectors
	//const string kds[] = { "surf", "mser" };

   // check if enough arguments are defined
   if (argc < 3){
      cout << "Usage:\n\tparameter1: path to imgage1"  << endl;
	  cout << "Usage:\n\tparameter2: path to imgage2" << endl;
      cout << "\nPress enter to exit"  << endl;
      cin.get();
      return -1;
   }

   // construct objects
   Controller controller;
   PreProcessing preProcessing;
   KeypointDetection keypointDetection;
   
    // load image, path in argv[1]
    cout << "load image1" << endl;
    Mat img = imread(argv[1], 0);
    if (!img.data){
      cout << "ERROR: original image1 not specified"  << endl;
      cout << "Press enter to exit..."  << endl;
      cin.get();
      return -1;
    }
	// load image, path in argv[2]
	cout << "load image2" << endl;
	Mat img2 = imread(argv[2], 0);
	if (!img2.data) {
		cout << "ERROR: original image2 not specified" << endl;
		cout << "Press enter to exit..." << endl;
		cin.get();
		return -1;
	}
	
    // convert U8 to 32F
    img.convertTo(img, CV_32FC1);
	img2.convertTo(img2, CV_32FC1);
    cout << " > done" << endl;

	// building a list for preprocessing images including original image1
	vector<string> pVec = controller.getPreProcessors();
	list<Mat> iList1;	
	if (controller.useOriginal()) iList1.push_back(img);
	if (controller.useHistEqual()) iList1.push_back(preProcessing.histogramEqualisation(img));
	if (controller.useClahe()) iList1.push_back(preProcessing.clahe(img));
	if (controller.useNlm()) iList1.push_back(preProcessing.nlmDenoising(img));
	if (controller.useBilateral()) iList1.push_back(preProcessing.bilateralFiltering(img));
	// building a list for preprocessing images including original image2
	list<Mat> iList2;
	if (controller.useOriginal()) iList2.push_back(img2);
	if (controller.useHistEqual()) iList2.push_back(preProcessing.histogramEqualisation(img2));
	if (controller.useClahe()) iList2.push_back(preProcessing.clahe(img2));
	if (controller.useNlm()) iList2.push_back(preProcessing.nlmDenoising(img2));
	if (controller.useBilateral()) iList2.push_back(preProcessing.bilateralFiltering(img2));
	
	//test Ransac with surf
	//Test test;
	//cout << "iList1: " << iList1.size() << " iList2: " << iList2.size() << " sList: " << pVec.size() << endl;
	//for (int i = 0; i < pVec.size(); ++i) {
	//	test.test(iList1.front(), iList2.front(), pVec[i]);
	//	iList1.pop_front();
	//	iList2.pop_front();
	//}
	
	
	// building a vector for keypoints for each image in iList1
	vector<string> kVec = controller.getKeypointDetectors();
	vector<vector<KeyPoint>> keyVector1;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine keypoints for " << pVec[i]<<"_1..." << endl;;
		if (controller.useSurf()) keyVector1.push_back(keypointDetection.surf(iList1.front(), (pVec[i] + "_surf_1_.dat").c_str(), false, true));
		if (controller.useMser()) keyVector1.push_back(keypointDetection.mser(iList1.front(), (pVec[i] + "_mser_1_.dat").c_str(), false, true));
		if (controller.useBrisk()) keyVector1.push_back(keypointDetection.brisk(iList1.front(), (pVec[i] + "_brisk_1_.dat").c_str(), false, true));
		if (controller.useFreak()) keyVector1.push_back(keypointDetection.freak(iList1.front(), (pVec[i] + "_freak_1_.dat").c_str(), false, true));
		if (controller.useOrb()) keyVector1.push_back(keypointDetection.orb(iList1.front(), (pVec[i] + "_orb_1_.dat").c_str(), false, true));
		iList1.pop_front();
	}
	// building a vector for keypoints for each image in iList2
	vector<vector<KeyPoint>> keyVector2;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine keypoints for " << pVec[i] << "_2..." << endl;;
		if (controller.useSurf()) keyVector2.push_back(keypointDetection.surf(iList2.front(), (pVec[i] + "_surf_2_.dat").c_str(), false, false));
		if (controller.useMser()) keyVector2.push_back(keypointDetection.mser(iList2.front(), (pVec[i] + "_mser_2_.dat").c_str(), false, false));
		if (controller.useBrisk()) keyVector2.push_back(keypointDetection.brisk(iList2.front(), (pVec[i] + "_brisk_2_.dat").c_str(), false, false));
		if (controller.useFreak()) keyVector2.push_back(keypointDetection.freak(iList2.front(), (pVec[i] + "_freak_2_.dat").c_str(), false, false));
		if (controller.useOrb()) keyVector1.push_back(keypointDetection.orb(iList2.front(), (pVec[i] + "_orb_2_.dat").c_str(), false, true));
		iList2.pop_front();
	}


	//number of preprocessing images including original image
	//int kvSize = keypointVector.size();
	//int startSize = imageList.size();
	//while (imageList.size() > 0) {
	//	//keypointDetection.showKeypoints(win.c_str(), keypointVector[i][j]);
	//	keypointDetection.run(imageList.front(), imageName[startSize - imageList.size()].c_str(), false, true);
	//	imageList.pop_front();
	//}

	////match keypoints trial
	////read file1 # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//string image3;
	//ifstream ifImage3 ("image3_kp.dat");
	//if (ifImage3.is_open()) {
	//	getline(ifImage3, image3);
	//	ifImage3.close();
	//}
	//istringstream isImage3(image3);
	//vector<string> resImage3;
	//vector<Point2f> vecImage3;
	////vecImage3.push_back(Point2f(1, 1));
	//for (string cur; getline(isImage3, cur, ','); resImage3.push_back(cur));
	//
	////Create vector<Point2f for ransac
	//for (int i = 0; i < resImage3.size(); ++i) {
	//	vecImage3.push_back(Point2f(stof(resImage3[i].substr(0, resImage3[i].find("-"))), stof(resImage3[i].substr(resImage3[i].find("-")+1))));
	//	//cout << "vecImage3[i]: " << vecImage3[i] << endl;
	//}

	////read file2 # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//string surf;
	//ifstream ifSurf("histEqual_mser.dat");
	//if (ifSurf.is_open()) {
	//	getline(ifSurf, surf);
	//	ifSurf.close();
	//}
	//istringstream isSurf(surf);
	//vector<string> resSurf;
	//vector<Point2f> vecSurf;
	//for (string cur; getline(isSurf, cur, ','); resSurf.push_back(cur));

	////Create vector<Point2f for ransac
	//for (int i = 0; i < resSurf.size(); ++i) {
	//	vecSurf.push_back(Point2f(stof(resSurf[i].substr(0, resSurf[i].find("-"))), stof(resSurf[i].substr(resSurf[i].find("-") + 1))));
	//	//cout << "veSurf[i]: " << vecSurf[i] << endl;
	//}

	//int hitsSurf = 0;
	//int scoreSurf = resSurf.size();
	//int tmp = scoreSurf;
	////while (tmp > 0) {
	//for (auto itS = resSurf.cbegin(); itS != resSurf.cend(); ++itS) {
	//	for (auto it = resImage3.cbegin(); it != resImage3.cend(); ++it) {
	//		if (*itS == *it) {
	//			hitsSurf++;
	//			break;
	//		}
	//	}
	//}
	////resSurf.pop_back();
	////}
	//cout << "Hits image3 - surf: " << hitsSurf << endl;
	//Mat h = findHomography(vecImage3, vecImage3, CV_RANSAC);

	
	//Test test;
	//test.test(img, img2, "Good Matches & Object detection");

	KeypointDescription keypointDescription;
	keypointDescription.orb(img, img2);
	keypointDescription.brisk(img, img2);
	keypointDescription.sift(img, img2);
	//keypointDescription.freak(img, img2);

	/*vector<Point2f> vp1, vp2;
	vp1.push_back(Point2f(1, 1));
	vp1.push_back(Point2f(100, 1));
	vp1.push_back(Point2f(50, 50));
	vp1.push_back(Point2f(20, 10));

	vp2.push_back(Point2f(11, 1));
	vp2.push_back(Point2f(110, 1));
	vp2.push_back(Point2f(60, 50));
	vp2.push_back(Point2f(30, 10));

	Mat H = findHomography(vp1, vp2, CV_RANSAC);
	cout << "ransac..." << endl << H << endl;
	vector<Point3f> v;
	v.push_back({ vp1.at(0).x, vp1.at(0).y,1 });
	v.push_back({ 2,2,1 });

	transform(v, v, H);
	cout << "correct: " << vp2.at(0) << " calculated: " << v.at(0) << endl;*/
	
	waitKey(0);
	
   return 0;
} 
