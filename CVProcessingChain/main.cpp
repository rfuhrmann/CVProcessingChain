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
#include "KeypointMatcher.h"
#include "Test.h"
#include <list>
#include <vector>
#include <string>
//#include<fstream>

using namespace std;
 
void buildPreprocessingVector(vector<Mat>& vec1, Mat& img1, vector<Mat>& vec2, Mat& img2) {
	Controller controller;
	PreProcessing preProcessing;

	if (controller.useOriginal() == true) {
		vec1.push_back(img1);
		vec2.push_back(img2);
	}
	if (controller.useHistEqual() == true) {
		vec1.push_back(preProcessing.histogramEqualisation(img1));
		vec2.push_back(preProcessing.histogramEqualisation(img2));
	}
	if (controller.useClahe() == true) {
		vec1.push_back(preProcessing.clahe(img1));
		vec2.push_back(preProcessing.clahe(img2));
	}
	if (controller.useNlm() == true) {
		vec1.push_back(preProcessing.nlmDenoising(img1));
		vec2.push_back(preProcessing.nlmDenoising(img2));
	}

	if (controller.useBilateral() == true) {
		vec1.push_back(preProcessing.bilateralFiltering(img1));
		vec2.push_back(preProcessing.bilateralFiltering(img2));
	}
}


// usage: path to image in argv[1]
// main function. loads image, calls preprocessing routines, calls keypoint detectors, records processing times
int main(int argc, char** argv) {

   // check if enough arguments are defined
   if (argc < 3){
      cout << "Usage:\n\tparameter1: path to imgage1"  << endl;
	  cout << "Usage:\n\tparameter2: path to imgage2" << endl;
	  cout << "Usage:\n\tparameter3: path to homography" << endl;
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
   KeypointMatcher keypointMatcher;
   fileManager.createEmptyJson("output.json");
   
    // load image, path in argv[1]
    cout << "load image1" << endl;
    Mat img1 = imread(argv[1], 0);
    if (!img1.data){
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
    //img1.convertTo(img1, CV_32FC1);
	//img2.convertTo(img2, CV_32FC1);
    cout << " > done" << endl;

	// building vectors for preprocessing images including original image
	vector<Mat> iVec1, iVec2;
	buildPreprocessingVector(iVec1, img1, iVec2, img2);
	
	// vector of all preprocessing steps including original image
	vector<string> pVec = controller.getPreProcessors();

	// #################### detection ####################
	// building a vector of keypoints for each image in iList
	vector<vector<KeyPoint>> kVecSift1, kVecSurf1, kVecMser1, kVecBrisk1, kVecFreak1, kVecOrb1;
	vector<vector<KeyPoint>> kVecSift2, kVecSurf2, kVecMser2, kVecBrisk2, kVecFreak2, kVecOrb2;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine keypoints for " << pVec[i]<<" ..." << endl;
		if (controller.useSift() == true) {
			kVecSift1.push_back(keypointDetection.sift(iVec1[i], (pVec[i] + "_sift_1.dat").c_str(), false, true));
			kVecSift2.push_back(keypointDetection.sift(iVec2[i], (pVec[i] + "_sift_2.dat").c_str(), false, false));
		}
		if (controller.useSurf() == true) {
			kVecSurf1.push_back(keypointDetection.surf(iVec1[i], (pVec[i] + "_surf_1.dat").c_str(), false, true));
			kVecSurf2.push_back(keypointDetection.surf(iVec2[i], (pVec[i] + "_surf_2.dat").c_str(), false, false));
		}
		if (controller.useMser() == true) {
			kVecMser1.push_back(keypointDetection.mser(iVec1[i], (pVec[i] + "_mser_1.dat").c_str(), false, true));
			kVecMser2.push_back(keypointDetection.mser(iVec2[i], (pVec[i] + "_mser_2.dat").c_str(), false, false));
		}
		if (controller.useBrisk() == true) {
			kVecBrisk1.push_back(keypointDetection.brisk(iVec1[i], (pVec[i] + "_brisk_1.dat").c_str(), false, true));
			kVecBrisk2.push_back(keypointDetection.brisk(iVec2[i], (pVec[i] + "_brisk_2.dat").c_str(), false, false));
		}
		if (controller.useFreak() == true) {
			kVecFreak1.push_back(keypointDetection.fast(iVec1[i], (pVec[i] + "_freak_1.dat").c_str(), false, true));
			kVecFreak2.push_back(keypointDetection.fast(iVec2[i], (pVec[i] + "_freak_2.dat").c_str(), false, false));
		}
		if (controller.useOrb() == true) {
			kVecOrb1.push_back(keypointDetection.orb(iVec1[i], (pVec[i] + "_orb_1.dat").c_str(), false, true));
			kVecOrb2.push_back(keypointDetection.orb(iVec2[i], (pVec[i] + "_orb_2.dat").c_str(), false, false));
		}
	}
	cout << " > done" << endl;


	// write keypoints to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write keypoints to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) {
			fileManager.writeKeypointsToJson(pVec[i] + "_sift_keypoints_1", kVecSift1[i]);
			fileManager.writeKeypointsToJson(pVec[i] + "_sift_keypoints_2", kVecSift2[i]);
		}
		if (controller.useSurf() == true) {
			fileManager.writeKeypointsToJson(pVec[i] + "_surf_keypoints_1", kVecSurf1[i]);
			fileManager.writeKeypointsToJson(pVec[i] + "_surf_keypoints_2", kVecSurf2[i]);
		}
		if (controller.useMser() == true) {
			fileManager.writeKeypointsToJson(pVec[i] + "_mser_keypoints_1", kVecMser1[i]);
			fileManager.writeKeypointsToJson(pVec[i] + "_mser_keypoints_2", kVecMser2[i]);
		}
		if (controller.useBrisk() == true) {
			fileManager.writeKeypointsToJson(pVec[i] + "_brisk_keypoints_1", kVecBrisk1[i]);
			fileManager.writeKeypointsToJson(pVec[i] + "_brisk_keypoints_2", kVecBrisk2[i]);
		}
		if (controller.useFreak() == true) {
			fileManager.writeKeypointsToJson(pVec[i] + "_freak_keypoints_1", kVecFreak1[i]);
			fileManager.writeKeypointsToJson(pVec[i] + "_freak_keypoints_2", kVecFreak2[i]);
		}
		if (controller.useOrb() == true) {
			fileManager.writeKeypointsToJson(pVec[i] + "_orb_keypoints_1", kVecOrb1[i]);
			fileManager.writeKeypointsToJson(pVec[i] + "_orb_keypoints_2", kVecOrb2[i]);
		}	
	}
	cout << " > done" << endl;

	// #################### description ####################
	// building a vector of descriptors for each image in iList1
	vector<vector<Mat>> descVecSift, descVecSurf, descVecBrisk, descVecFreak, descVecOrb;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine descriptors for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) descVecSift.push_back(keypointDescription.sift(iVec1[i], kVecSift1[i], iVec2[i], kVecSift2[i]));
		if (controller.useSurf() == true) descVecSurf.push_back(keypointDescription.surf(iVec1[i], kVecSurf1[i], iVec2[i], kVecSurf2[i]));
		if (controller.useBrisk() == true) descVecBrisk.push_back(keypointDescription.brisk(iVec1[i], kVecBrisk1[i], iVec2[i], kVecBrisk2[i]));
		if (controller.useFreak() == true) descVecFreak.push_back(keypointDescription.freak(iVec1[i], kVecFreak1[i], iVec2[i], kVecFreak2[i]));
		if (controller.useOrb() == true) descVecOrb.push_back(keypointDescription.orb(iVec1[i], kVecOrb1[i], iVec2[i], kVecOrb2[i]));
	}
	cout << " > done" << endl;

	// #################### match ####################
	// #################### filter by crosscheck ####################
	// #################### filter by ratio ####################
	// building a vector of matches for each image-pair
	vector<vector<DMatch>> matchVecSift, matchVecSurf, matchVecBrisk, matchVecFreak, matchVecOrb;
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "determine matches for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) matchVecSift.push_back(keypointMatcher.ratioMatcher(NORM_L2, descVecSift[i][0], descVecSift[i][1]));
		if (controller.useSurf() == true) matchVecSurf.push_back(keypointMatcher.ratioMatcher(NORM_L2, descVecSurf[i][0], descVecSurf[i][1]));
		if (controller.useBrisk() == true) matchVecBrisk.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, descVecBrisk[i][0], descVecBrisk[i][1]));
		if (controller.useFreak() == true) matchVecFreak.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, descVecFreak[i][0], descVecFreak[i][1]));
		if (controller.useOrb() == true) matchVecOrb.push_back(keypointMatcher.ratioMatcher(NORM_HAMMING, descVecOrb[i][0], descVecOrb[i][1]));

	}
	cout << " > done" << endl;

	// #################### filter by threshold ####################
	// filter matches by threshold
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by threshold for " << pVec[i] << " ..." << endl;
		cout << "thresh: " << controller.getThreshold() << endl;
		if (controller.useSift() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecSift[i]);
		if (controller.useSurf() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecSurf[i]);
		if (controller.useBrisk() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecBrisk[i]);
		if (controller.useFreak() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecFreak[i]);
		if (controller.useOrb() == true) keypointMatcher.thresholdFilter(controller.getThreshold(), matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// #################### filter by ransac ####################
	// filter matches by ransac
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by ransac for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) keypointDescription.ransacFilter(kVecSift1[i], kVecSift2[i], matchVecSift[i]);
		if (controller.useSurf() == true) keypointDescription.ransacFilter(kVecSurf1[i], kVecSurf2[i], matchVecSurf[i]);
		if (controller.useBrisk() == true) keypointDescription.ransacFilter(kVecBrisk1[i], kVecBrisk2[i], matchVecBrisk[i]);
		if (controller.useFreak() == true) keypointDescription.ransacFilter(kVecFreak1[i], kVecFreak2[i], matchVecFreak[i]);
		if (controller.useOrb() == true) keypointDescription.ransacFilter(kVecOrb1[i], kVecOrb2[i], matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// write matches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write matches to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_matches", "matches", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i]+"_surf_matches", "matches", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_matches", "matches", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_matches", "matches", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_matches", "matches", matchVecOrb[i]);
	}
	cout << " > done" << endl;

	// #################### filter by real matches ####################
	// filter matches by known homography
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "filter matches by known homography for " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) keypointMatcher.homographyFilter(kVecSift1[i], kVecSift2[i], matchVecSift[i], H);
		if (controller.useSurf() == true) keypointMatcher.homographyFilter(kVecSurf1[i], kVecSurf2[i], matchVecSurf[i], H);
		if (controller.useBrisk() == true) keypointMatcher.homographyFilter(kVecBrisk1[i], kVecBrisk2[i], matchVecBrisk[i], H);
		if (controller.useFreak() == true) keypointMatcher.homographyFilter(kVecFreak1[i], kVecFreak2[i], matchVecFreak[i], H);
		if (controller.useOrb() == true) keypointMatcher.homographyFilter(kVecOrb1[i], kVecOrb2[i], matchVecOrb[i], H);
	}
	cout << " > done" << endl;

	// write realMatches to json
	for (int i = 0; i < pVec.size(); ++i) {
		cout << "write realMatches to json " << pVec[i] << " ..." << endl;
		if (controller.useSift() == true) fileManager.writeMatchesToJson(pVec[i] + "_sift_realMatches", "realMatches", matchVecSift[i]);
		if (controller.useSurf() == true) fileManager.writeMatchesToJson(pVec[i] + "_surf_realMatches", "realMatches", matchVecSurf[i]);
		if (controller.useBrisk() == true) fileManager.writeMatchesToJson(pVec[i] + "_brisk_realMatches", "realMatches", matchVecBrisk[i]);
		if (controller.useFreak() == true) fileManager.writeMatchesToJson(pVec[i] + "_freak_realMatches", "realMatches", matchVecFreak[i]);
		if (controller.useOrb() == true) fileManager.writeMatchesToJson(pVec[i] + "_orb_realMatches", "realMatches", matchVecOrb[i]);
	}
	cout << " > done" << endl;

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


