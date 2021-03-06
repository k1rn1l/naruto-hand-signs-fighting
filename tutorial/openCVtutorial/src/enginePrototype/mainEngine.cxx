/*
 * mainEngine.cxx
 *
 *  Created on: Nov 13, 2010
 *      Author: michele
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv/cxtypes.h>
#include <opencv/cxcore.h>   // unnecessary - included in cv.h

#include <iostream>
#include <stdio.h>

#include "RecognitionEngine.h"
#include "engineModules/EngineModule.h"
//////////////////
//My built modules
//////////////////
#include "engineModules/CannyEM.h"
#include "engineModules/LaplacianEM.h"
#include "engineModules/SobelEM.h"
#include "engineModules/HistogramEM.h"
#include "engineModules/ContoursFinderEM.h"
#include "engineModules/BlurEM.h"
//////////////////

#define MAIN_WIN "mainWin"
#define RES_WIN "resWin"
#define TEMPLATE_NAME "TEMPLATE-tiger.png"

const char* IMAGE_PATH = "./2010-11-02-093111.png";
static bool CREATE_TEMPLATE = true;


int main(int argc, char* argv[] ){
	IplImage *temp, *img, *res;
	if(argc<2){
		printf("Usage: main <image-file-name>\n\7");
		exit(0);
	}
	temp = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	img = cvCreateImage(cvSize(temp->width, temp->height), IPL_DEPTH_8U, 1);
	cvConvertScale(temp, img);
	res = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_32F, img->nChannels);

	////////////////////////
	//The first step is to add different modules to our engine
	///////////////////////

	HistogramEM *hist = new HistogramEM();
	//If you want to display the histogram...
	hist->createHistogram(img);
	cvNamedWindow("histWin", CV_WINDOW_AUTOSIZE);
	hist->displayHistogram("histWin");

	//equalization
	recognitionEngine.addModule(hist);

	//Sobel
//	recognitionEngine.addModule(new SobelEM(7, 2, 2));
	recognitionEngine.addModule(new SobelEM(CV_SCHARR, 1, 0)); //<-
//	recognitionEngine.addModule(new SobelEM(CV_SCHARR, 0, 1));

	//Canny
//	recognitionEngine.addModule(new CannyEM(5, 200, 40));

	//blurring
	recognitionEngine.addModule(new BlurEM(CV_GAUSSIAN,3,3)); //<-(?)

	//Laplacian
	recognitionEngine.addModule(new LaplacianEM(7)); //<-

	//contours extraction
//	recognitionEngine.addModule(new ContoursFinderEM(120));

	/////////////////////////////////////
	//Processing phase:
	cout<<"Starting process\n";
	recognitionEngine.process(img, res);
	///////////////////////////////////

	if(CREATE_TEMPLATE){
		cout<<"creating template\n";
		cvSaveImage(TEMPLATE_NAME, res);
	}

//	recognitionEngine.evaluate(res, tiger);


	cvNamedWindow(MAIN_WIN, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(MAIN_WIN, 50, 50);

	// create a second window
	cvNamedWindow(RES_WIN, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(RES_WIN, 700, 50);

	cvShowImage(MAIN_WIN, img);
	cvShowImage(RES_WIN, res);


	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&res);
	cvDestroyAllWindows();
	return 0;
}
