//============================================================================
// Name        : PreProessing.cpp
// Author      : Robert Fuhrmann
// Version     : 1.0
// Copyright   : -
// Description : provedes preprocessing functions
//============================================================================

#include "PreProcessing.h"



// function calls preprocessing functions
/*
in                   :  input image
return               :  list of input image and all preprocessed images
*/
list<Mat> PreProcessing::run(Mat& img){
	list<Mat> images;
	// Mat::zeros(img.size(), img.type());
	images.push_back(img);
	images.push_back(histogramEqualisation(img));
	//images.push_back(clahe(img));
	//images.push_back(nlmDenoising(img));
	//images.push_back(bilateralFiltering(img));
	return images;

}

// histogram equalization for global contrat enhancement
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::histogramEqualisation(Mat& img) {
	//Mat img2 = Mat::zeros(img.size(), img.type());
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);
	equalizeHist(img2, img2);
	return img2;
}


// clahe for local contrat enhancement
//Contrast Limited Adaptive Histogram Equalization
/*
img         :  input image
return      :  the result image
*/
Mat PreProcessing::clahe(Mat& img) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);

	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(8);
	clahe->apply(img2, img2);
	return img2;
}

// bilateral Filter for blurring whilst keeping strong edges
/*
img         :  input image
return      :  the blurred image
*/
Mat PreProcessing::bilateralFiltering(Mat& img) {
	Mat img2 = img.clone();
	Mat img3 = Mat::zeros(img2.size(), img2.type());
	img2.convertTo(img2, CV_8UC1);
	bilateralFilter(img2, img3, 5, 80, 80);
	return img3;
}

// non local means filter for blurring
/*
img         :  input image
return      :  the blurred image
*/
Mat PreProcessing::nlmDenoising(Mat& img) {
	Mat img2 = img.clone();
	img2.convertTo(img2, CV_8UC1);
	fastNlMeansDenoising(img2, img2, 3, 7, 5); //prefered: 3,7,21
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


