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


//The conventional ranges for R, G, and B channel values are :
//	0 to 255 for CV_8U images
//	0 to 65535 for CV_16U images
//	0 to 1 for CV_32F images



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
	//img2 = (planes[0]/3 + planes[1]/3 + planes[2]/3);
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
	//img2.convertTo(img2, CV_8UC1);
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
// histogram equalization for global contrat enhancement
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::histogramEqualisation(Mat& img) {
	Mat img2;// = weightedGray(img);
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
	Mat img2;// = weightedGray(img);
	//img2.convertTo(img2, CV_8UC1);

	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(8);
	clahe->apply(img, img2);
	return img2;
}

// bilateral Filter for blurring whilst keeping strong edges
/*
img         :  input image
return      :  the blurred image
*/
Mat PreProcessing::bilateralFiltering(Mat& img) {
	//Mat img2;// = weightedGray(img);
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
	Mat img2;// = weightedGray(img);
	fastNlMeansDenoising(img, img2, 3, 7, 5); //prefered: 3,7,21
	return img2;
}

// block matching and 3d filtering
/*
img         :  input image
return      :  the denoised image
*/
Mat PreProcessing::bm3d(Mat& img) {
	Mat img2;// = weightedGray(img);
	xphoto::bm3dDenoising(img, img2, 1.0, 4, 16, 2500, 400, 8, 1, 2.0, 4, 0, 0);
	return img2;
}

// function degrades the given image with gaussian blur and additive gaussian noise
/*
img         :  input image
degradedImg :  degraded output image
filterDev   :  standard deviation of kernel for gaussian blur
snr         :  signal to noise ratio for additive gaussian noise
return      :  the used gaussian kernel
*/
Mat PreProcessing::degradeImage(Mat& img, Mat& degradedImg, double filterDev, double snr){

    int kSize = round(filterDev*3)*2 - 1;
   
    Mat gaussKernel = getGaussianKernel(kSize, filterDev, CV_32FC1);
    gaussKernel = gaussKernel * gaussKernel.t();

    Mat imgs = img.clone();
    dft( imgs, imgs, CV_DXT_FORWARD, img.rows);
    Mat kernels = Mat::zeros( img.rows, img.cols, CV_32FC1);
    int dx, dy; dx = dy = (kSize-1)/2.;
    for(int i=0; i<kSize; i++) for(int j=0; j<kSize; j++) kernels.at<float>((i - dy + img.rows) % img.rows,(j - dx + img.cols) % img.cols) = gaussKernel.at<float>(i,j);
	dft( kernels, kernels, CV_DXT_FORWARD );
	mulSpectrums( imgs, kernels, imgs, 0 );
	dft( imgs, degradedImg, CV_DXT_INV_SCALE, img.rows );
	
    Mat mean, stddev;
    meanStdDev(img, mean, stddev);

    Mat noise = Mat::zeros(img.rows, img.cols, CV_32FC1);
    randn(noise, 0, stddev.at<double>(0)/snr);
    degradedImg = degradedImg + noise;
    threshold(degradedImg, degradedImg, 255, 255, CV_THRESH_TRUNC);
    threshold(degradedImg, degradedImg, 0, 0, CV_THRESH_TOZERO);

    return gaussKernel;
}

// Function displays image (after proper normalization)
/*
win   :  Window name
img   :  Image that shall be displayed
cut   :  whether to cut or scale values outside of [0,255] range
*/
void PreProcessing::showImage(const char* win, Mat img, bool cut){

   Mat tmp = img.clone();

   if (tmp.channels() == 1){
      if (cut){
         threshold(tmp, tmp, 255, 255, CV_THRESH_TRUNC);
         threshold(tmp, tmp, 0, 0, CV_THRESH_TOZERO);
      }else
         normalize(tmp, tmp, 0, 255, CV_MINMAX);
         
      tmp.convertTo(tmp, CV_8UC1);
   }else{
      tmp.convertTo(tmp, CV_8UC3);
   }
   imshow(win, tmp);
}


