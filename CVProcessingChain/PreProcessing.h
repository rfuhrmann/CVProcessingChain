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
        
      
      void test(void);
	  //change colorspace
	  Mat gray(Mat&);
	  Mat weightedGray(Mat&);
	  Mat yCrCb(Mat&);
	  Mat luv(Mat&);
	  Mat histogramEqualisation(Mat&);//contrast enhancement global
	  Mat clahe(Mat&);//contrast enhancement lokal
	  Mat nlmDenoising(Mat&);//denoising
	  Mat bilateralFiltering(Mat&);//blur

	  //convert image to 
	  

      // function headers of given functions
      Mat degradeImage(Mat& img, Mat& degradedImg, double filterDev, double snr);
      void showImage(const char* win, Mat img, bool cut=true);

   private:
    
      // testing routines

};
