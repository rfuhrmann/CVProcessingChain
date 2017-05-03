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
#include<list>
#include<vector>
//#include<fstream>

using namespace std;


// usage: path to image in argv[1]
// main function. loads image, calls preprocessing routines, calls keypoint detectors, records processing times
int main(int argc, char** argv) {

	//Controller controller;
	
	//definition of used preprocessing methods
	const string imageName[] = { "original", "histEqual" };
	//definition of used keypoint detectors
	//const string kds[] = { "surf", "mser" };

   // check if enough arguments are defined
   if (argc < 2){
      cout << "Usage:\n\tparameter1: path to imgage"  << endl;
      cout << "\nPress enter to exit"  << endl;
      cin.get();
      return -1;
   }

   // construct processing object
   PreProcessing preProcessing;
   KeypointDetection keypointDetection;
   
    // load image, path in argv[1]
    cout << "load image" << endl;
    Mat img = imread(argv[1], 0);
    if (!img.data){
      cout << "ERROR: original image not specified"  << endl;
      cout << "Press enter to exit..."  << endl;
      cin.get();
      return -1;
    }
	
    // convert U8 to 32F
    img.convertTo(img, CV_32FC1);
    cout << " > done" << endl;

	//keypointDetection.test();
	//vector<Mat> imageVector (preProcessing.run(img));
	list<Mat> imageList(preProcessing.run(img));
	//vector<vector<Mat>> keypointVector;

	//number of preprocessing images including original image
	//int kvSize = keypointVector.size();
	int startSize = imageList.size();
	while (imageList.size() > 0) {
		//keypointDetection.showKeypoints(win.c_str(), keypointVector[i][j]);
		keypointDetection.run(imageList.front(), imageName[startSize - imageList.size()].c_str(), true, true);
		imageList.pop_front();
	}

	//match keypoints trial
	//read file1 # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	string image3;
	ifstream ifImage3 ("image3_kp.dat");
	if (ifImage3.is_open()) {
		getline(ifImage3, image3);
		ifImage3.close();
	}
	istringstream isImage3(image3);
	vector<string> resImage3;
	for (string cur; getline(isImage3, cur, ','); resImage3.push_back(cur));
	//cout << result[1] << endl;
	cout << "#image3: " << resImage3.size() << endl;
	//read file2 # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	string surf;
	ifstream ifSurf("histEqual_mser.dat");
	if (ifSurf.is_open()) {
		getline(ifSurf, surf);
		ifSurf.close();
	}
	istringstream isSurf(surf);
	vector<string> resSurf;
	for (string cur; getline(isSurf, cur, ','); resSurf.push_back(cur));
	cout << "#surf: " << resSurf.size() << endl;

	int hitsSurf = 0;
	int scoreSurf = resSurf.size();
	int tmp = scoreSurf;
	//while (tmp > 0) {
	for (auto itS = resSurf.cbegin(); itS != resSurf.cend(); ++itS) {
		for (auto it = resImage3.cbegin(); it != resImage3.cend(); ++it) {
			if (*itS == *it) {
				hitsSurf++;
				break;
			}
		}
	}
	//resSurf.pop_back();
	//}
	cout << "Hits image3 - surf: " << hitsSurf << endl;


	////number of preprocessing images including original image
	//int kvSize = keypointVector.size();
	//string win;
	//for (int i = 0; i < kvSize; i++) {
	//	//number of keypoint detectors
	//	int kvvSize = keypointVector[i].size();
	//	for (int j = 0; j < kvvSize; j++) {
	//		win = ppm[i] + '_' + kds[j];
	//		keypointDetection.showKeypoints(win.c_str(), keypointVector[i][j]);
	//		//ppm[i]+'_'+ kds[j]
	//		//cout << ppm[i] + "_" + kds[j] << endl;
	//	}
	//}
	

	//vector<Mat> imageList(keypointDetection.run(img));
	//cout << "imageList.size:" << imageList.size() << endl;
	//cout << "keypointVector.size:" << keypointVector.size() << endl;
	//cout << "img:" << argv[1] << endl;

    // wait
	preProcessing.showImage(argv[1], img);
    waitKey(0);
	
   return 0;
} 
