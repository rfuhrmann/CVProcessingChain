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
#include "FileManager.h"
#include "Test.h"
#include<list>
#include<vector>
#include<string>
//#include<fstream>

using namespace std;


// usage: path to image in argv[1]
// main function. loads image, calls preprocessing routines, calls keypoint detectors, records processing times
int main(int argc, char** argv) {

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
   KeypointDescription keypointDescription;
   FileManager fileManager;
   fileManager.createEmptyJson("output.json");
   
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
	// load homography, path in argv[3]
	cout << "load homography" << endl;
	Mat H = (Mat_<float>(3, 3) << 7.6285898e-01, -2.9922929e-01, 2.2567123e+02, 3.3443473e-01, 1.0143901e+00, -7.6999973e+01, 3.4663091e-04, -1.4364524e-05, 1.0000000e+00);
	

	//ifstream f;  // Datei-Handle
	//string s;
	//f.open(argv[3], ios::in); // Öffne Datei aus Parameter
	//while (!f.eof())          // Solange noch Daten vorliegen
	//{
	//	getline(f, s);        // Lese eine Zeile
	//	cout << s << endl;    // Zeige sie auf dem Bildschirm

	//}
	//f.close();

	
    // convert U8 to 32F
    img.convertTo(img, CV_32FC1);
	img2.convertTo(img2, CV_32FC1);
    cout << " > done" << endl;

	// building a list for preprocessing images including original image1
	list<Mat> iList1;
	vector<Mat> iVec1;
	if (controller.useOriginal()) iVec1.push_back(img);
	if (controller.useHistEqual()) iVec1.push_back(preProcessing.histogramEqualisation(img));
	if (controller.useClahe()) iVec1.push_back(preProcessing.clahe(img));
	if (controller.useNlm()) iVec1.push_back(preProcessing.nlmDenoising(img));
	if (controller.useBilateral()) iVec1.push_back(preProcessing.bilateralFiltering(img));
	// building a list for preprocessing images including original image2
	list<Mat> iList2;
	vector<Mat> iVec2;
	if (controller.useOriginal()) iVec2.push_back(img2);
	if (controller.useHistEqual()) iVec2.push_back(preProcessing.histogramEqualisation(img2));
	if (controller.useClahe()) iVec2.push_back(preProcessing.clahe(img2));
	if (controller.useNlm()) iVec2.push_back(preProcessing.nlmDenoising(img2));
	if (controller.useBilateral()) iVec2.push_back(preProcessing.bilateralFiltering(img2));
	//for matching
	list<Mat> mList1 = iList1;
	list<Mat> mList2 = iList2;

	
	// vector of all preprocessing steps including original image
	vector<string> pVec = controller.getPreProcessors();
	// building a vector of keypoints for each image in iList1
	//vector<string> kVec = controller.getKeypointDetectors();
	vector<vector<KeyPoint>> kVecSurf1, kVecMser1, kVecBrisk1, kVecFreak1, kVecOrb1;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine keypoints for " << pVec[i]<<"_1 ..." << endl;
		if (controller.useSurf()) kVecSurf1.push_back(keypointDetection.surf(iVec1[i], (pVec[i] + "_surf_1_.dat").c_str(), false, true));
		if (controller.useMser()) kVecMser1.push_back(keypointDetection.mser(iVec1[i], (pVec[i] + "_mser_1_.dat").c_str(), false, true));
		if (controller.useBrisk()) kVecBrisk1.push_back(keypointDetection.brisk(iVec1[i], (pVec[i] + "_brisk_1_.dat").c_str(), false, true));
		if (controller.useFreak()) kVecFreak1.push_back(keypointDetection.freak(iVec1[i], (pVec[i] + "_freak_1_.dat").c_str(), false, true));
		if (controller.useOrb()) kVecOrb1.push_back(keypointDetection.orb(iVec1[i], (pVec[i] + "_orb_1_.dat").c_str(), false, true));
	}
	// building a vector of keypoints for each image in iList2
	vector<vector<KeyPoint>> kVecSurf2, kVecMser2, kVecBrisk2, kVecFreak2, kVecOrb2;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine keypoints for " << pVec[i] << "_2 ..." << endl;
		if (controller.useSurf()) kVecSurf2.push_back(keypointDetection.surf(iVec2[i], (pVec[i] + "_surf_2_.dat").c_str(), false, false));
		if (controller.useMser()) kVecMser2.push_back(keypointDetection.mser(iVec2[i], (pVec[i] + "_mser_2_.dat").c_str(), false, false));
		if (controller.useBrisk()) kVecBrisk2.push_back(keypointDetection.brisk(iVec2[i], (pVec[i] + "_brisk_2_.dat").c_str(), false, false));
		if (controller.useFreak()) kVecFreak2.push_back(keypointDetection.freak(iVec2[i], (pVec[i] + "_freak_2_.dat").c_str(), false, false));
		if (controller.useOrb()) kVecOrb2.push_back(keypointDetection.orb(iVec2[i], (pVec[i] + "_orb_2_.dat").c_str(), false, true));
	}


	// building a vector of matches for each image-pair
	vector<vector<DMatch>> matchVector;
	vector<vector<DMatch>> matchVecSurf, matchVecOrb;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine matches for " << pVec[i] << " ..." << endl;
		if (controller.useSurf()) matchVecSurf.push_back(keypointDescription.surf(iVec1[i], kVecSurf1[i], iVec2[i], kVecSurf2[i]));
		if (controller.useOrb()) matchVecOrb.push_back(keypointDescription.orb(iVec1[i], kVecOrb1[i], iVec2[i], kVecOrb2[i]));
	}


	// filter matches by ransac
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by ransac for " << pVec[i] << " ..." << endl;
		if (controller.useSurf()) keypointDescription.ransacFilter(kVecSurf1[i], kVecSurf2[i], matchVecSurf[i]);
		if (controller.useOrb()) keypointDescription.ransacFilter(kVecOrb1[i], kVecOrb2[i], matchVecOrb[i]);
	}

	// write matches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write matches to json " << pVec[i] << " ..." << endl;
		if (controller.useSurf()) fileManager.writeMatchesToJson(pVec[i]+"_surf_matches", matchVecSurf[i]);
		if (controller.useOrb()) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", matchVecOrb[i]);
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

	
	//keypointDescription.orb(img, img2, H);
	//keypointDescription.brisk(img, img2);
	//keypointDescription.sift(img, img2);
	//keypointDescription.freak(img, img2);



	//vector<KeyPoint> keypoints;
	//fileManager.writeKeypointsToJson("detektor2", keypoints);
	
	waitKey(0);
	
   return 0;
} 


