//============================================================================
// Name        : PreProcessing.h
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : header file for PreProcessing steps
//============================================================================

#include <iostream>
#include<list>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PreProcessing{

   public:
      // constructor
	   PreProcessing(void){};
      // destructor
      ~PreProcessing(void){};
        
      // processing routines
      // start image restoration

      // testing routine
      void test(void);
	  list<Mat> run(Mat&);
	  //contrast enhancement
	  Mat histogramEqualisation(Mat&);
	  Mat clahe(Mat&);
	  //denoising
	  Mat nlmDenoising(Mat&);
	  //blur
	  Mat bilateralFiltering(Mat&);
      // function headers of given functions
      Mat degradeImage(Mat& img, Mat& degradedImg, double filterDev, double snr);
      void showImage(const char* win, Mat img, bool cut=true);

   private:
    
      // testing routines

};
