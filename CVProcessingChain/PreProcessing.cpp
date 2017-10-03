//============================================================================
// Name        : PreProessing.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : provedes preprocessing functions
//============================================================================

#include "PreProcessing.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xphoto/bm3d_image_denoising.hpp>
#include <opencv2/videostab/deblurring.hpp>
#include "DeblurAndrey94.h";



// grayscale conversion from BGR color space
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::gray(Mat& img) {
	Mat img2 = Mat::zeros(img.size(), CV_8UC1);
	vector<Mat> planes; 
	split(img, planes);
	divide(planes[0], 3, planes[0]);
	divide(planes[1], 3, planes[1]);
	divide(planes[2], 3, planes[2]);
	add(planes[0], planes[1], img2);
	add(img2, planes[2], img2);
	return img2;
}

// weighted grayscale conversion from BGR color space
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::weightedGray(Mat& img) {
	Mat img2;
	cvtColor(img, img2, CV_BGR2GRAY);
	return img2;
}

// grayscale conversion from YCrCb color space
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::yCrCb(Mat& img) {
	Mat img2;
	vector<Mat> planes;
	cvtColor(img, img2, COLOR_BGR2YCrCb);
	split(img2, planes);
	return planes[0];
}

// grayscale conversion from Luv color space
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::luv(Mat& img) {
	Mat img2;
	vector<Mat> planes;
	cvtColor(img, img2, COLOR_BGR2Luv);
	split(img2, planes);
	return planes[0];
}

// grayscale conversion from Lab color space
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::lab(Mat& img) {
	Mat img2;
	vector<Mat> planes;
	cvtColor(img, img2, COLOR_BGR2Lab);
	split(img2, planes);
	return planes[0];
}

// histogram equalization for global contrat enhancement
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::histogramEqualisation(Mat& img) {
	Mat img2;
	equalizeHist(img, img2);
	return img2;
}


// clahe for local contrat enhancement
//Contrast Limited Adaptive Histogram Equalization
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::clahe(Mat& img) {
	Mat img2;
	Ptr<CLAHE> clahe = createCLAHE(40, Size(8, 8));
	clahe->apply(img, img2);
	return img2;
}

// bilateral Filter for blurring whilst keeping strong edges
/*
img         :  input image
return      :  the blurred image
*/
Mat PreProcessing::bilateralFiltering(Mat& img) {
	Mat img2 = Mat::zeros(img.size(), img.type());
	bilateralFilter(img, img2, 5, 80, 80);
	return img2;
}

// non local means filter for blurring
/*
img         :  input image
return      :  the blurred image
*/
Mat PreProcessing::nlmDenoising(Mat& img) {
	Mat img2;
	fastNlMeansDenoising(img, img2, 3, 7, 21);
	return img2;
}

// block matching and 3d filtering
/*
img         :  input image
return      :  the denoised image
*/
Mat PreProcessing::bm3d(Mat& img) {
	Mat img2;
	xphoto::bm3dDenoising(img, img2, 1.0, 4, 16, 2500, 400, 8, 1, 2.0, 4, 0, 0);
	return img2;
}

// blind deblurring
/*
img         :  input image
return      :  the denoised image
*/
Mat PreProcessing::deblur(Mat& img) {
	cout << "deblur..." << endl;
	Mat img2 = img.clone();
	//videostab::WeightingDeblurer *deblurrer = new videostab::WeightingDeblurer();
	//deblurrer->deblur(0, img2);
	//cout << "deblur done" << endl;
	//cout<< img2.size()<<endl;
	//*deblurrer.deblur(1, img2);
	DeblurAndrey94 deblurAndrey94;
	img2 = deblurAndrey94.deblur(img2);
	return img2;
}


