//============================================================================
// Name        : main.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : calls preprocessing, keypoint detectors and test routines
//============================================================================

#include <iostream>

#include "PreProcessing.h"
#include "KeypointDetection.h"
#include<list>
#include<vector>

using namespace std;

// usage: path to image in argv[1]
// main function. loads image, calls preprocessing routines, calls keypoint detectors, records processing times
int main(int argc, char** argv) {

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

   // run test routines
   // NOTE: uncomment the following line for debugging/testing purposes!
   //preProcessing.test();
   
	// some windows for displaying imagesvoid degradeImage(Mat imgIn32F, Mat degradedImg, double filterDev, double snr)
    const char* win_1 = "Original Image";
    const char* win_2 = "histogram equalization image";
    const char* win_3 = "clahe";
    const char* win_4 = "fast nlm Denoising";
	const char* win_5 = "bilateral filtering";
    namedWindow( win_1 );
    namedWindow( win_2 );
    namedWindow( win_3 );
    namedWindow( win_4 );
	namedWindow( win_5);
   
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

	keypointDetection.test();
	//vector<Mat> imageVector (preProcessing.run(img));
	list<Mat> imageList(preProcessing.run(img));
	vector<vector<Mat>> keypointVector;

	while (imageList.size() > 0) {
		keypointVector.push_back(keypointDetection.run(imageList.front()));
		imageList.pop_front();
	}

	//vector<Mat> imageList(keypointDetection.run(img));
	cout << "imageList.size:" << imageList.size() << endl;
	cout << "keypointVector.size:" << keypointVector.size() << endl;
	//cout << "keypointVector:" << keypointVector.at(1).at(0) << endl;
    // show and safe gray-scale version of original image
	preProcessing.showImage( win_1, img);
	preProcessing.showImage(win_2, keypointVector.at(1).at(0));
	preProcessing.showImage(win_3, keypointVector.at(2).at(0));
	preProcessing.showImage(win_4, keypointVector.at(3).at(0));
	preProcessing.showImage(win_5, keypointVector.at(4).at(0));
    //imwrite( "original.png", img );
	
  
    //// degrade image
    //cout << "degrade image" << endl;
    //double snr = atof(argv[2]);
    //double filterDev = atof(argv[3]);
    //Mat degradedImg;
    //Mat gaussKernel = preProcessing.degradeImage(img, degradedImg, filterDev, snr);
    //cout << " > done" << endl;
    
    // wait
    waitKey(0);

   return 0;
} 
