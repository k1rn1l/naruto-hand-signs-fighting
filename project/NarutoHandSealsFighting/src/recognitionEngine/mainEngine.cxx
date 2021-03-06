/*
 * mainEngine.cxx
 *
 *  Created on: Nov 13, 2010
 *      Author: michele
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>   // unnecessary - included in cv.h

#include <iostream>
#include <stdio.h>

#include "RecognitionEngine.h"
#include "engineModules/EngineModule.h"
//////////////////
//My built modules
//////////////////
#include "engineModules/All.h"  // new trendy and 'in' way... for young
//#include "engineModules/CannyEM.h"  //old fashioned way... too looser... ;)
//#include "engineModules/LaplacianEM.h"
//#include "engineModules/SobelEM.h"
//#include "engineModules/HistogramEM.h"
//#include "engineModules/ContoursFinderEM.h"
//#include "engineModules/BlurEM.h"
//////////////////

//////////////////
//Evaluation functions
//////////////////
#include "evaluationFunctions/MulEvaluator.h"
//////////////////

#include "../gameLogic/SealsFactory.h"
#include "../gameLogic/SealsMap.h"
#include "../gameLogic/MovesFactory.h"
#include "../gameLogic/MovesSet.h"

#include "../DebugPrint.h"

#define TEMPLATE_WIN "Template"
#define INPUT_WIN "Input"
#define TEMPLATE_NAME "TEMPLATE-tiger.png"



void initEngine();

static bool CREATE_TEMPLATE = true;

//HistogramEM *hist = new HistogramEM();
//////////////////////////
/// CRUCIAL VARIABLES ////
MovesSet myMoveSet;
SealsMap smap;
//////////////////////////

/* Let's try to implement a tutorial for the
 * "Lightning Blade" move (in Italian "Mille Falchi")
*/
int main(int argc, char* argv[] ){
	IplImage *temp, *img, *res;

	//////////////////////////
	// Before doing anything else we have to use the factories
	// to build the seals and all the moves
	debugPrint("sealsFactoy\n");
	sealsFactory->buildSealsMap(&smap);
	debugPrint("movesFactory\n");
	movesFactory->buildMovesSet(&myMoveSet, &smap);
	//////////////////////////

	if(argc<4){
		printf("************************\n");
		printf("Lightning Blade tutorial\n");
		printf("SEALS: cow hare monkey\n");
		printf("************************\n");
		printf("Usage: %s <seal-1> <seal-2> <seal-3>\n\7", argv[0]);
		exit(0);
	}
	//If you want to display the histogram...
//		hist->createHistogram(img);
//		cvNamedWindow("histWin", CV_WINDOW_AUTOSIZE);
//		hist->displayHistogram("histWin");


	////////////////////////
	//The first step is to add different modules to our engine
	///////////////////////
	debugPrint("recognition engine initialization...");
//	initEngine();
	recognitionEngine->initEngine();
	debugPrint("done\n");

	////////////////////////
	//Then let's define the currentMove
	////////////////////////
	debugPrint("set current move...");
	recognitionEngine->setCurrentMove(myMoveSet.getMove("Lightning Blade"));
	debugPrint("done\n");
//	cout<<"current move is: "<<recognitionEngine->getCurrentMove()->getMoveName()<<"\n";
	debugPrint("current move is %s \n",
			recognitionEngine->getCurrentMove()->getMoveName().c_str());

	debugPrint("set evaluator function...");
	recognitionEngine->setEvaluatorFunction(mulEvaluator);
	debugPrint("done\n");

	cvNamedWindow(TEMPLATE_WIN, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(TEMPLATE_WIN, 50, 50);

	// create a second window
	cvNamedWindow(INPUT_WIN, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(INPUT_WIN, 700, 50);

	////////////////////////
	//For all inserted images...
	////////////////////////
	for(int i=1; i<argc; i++){
		cout<<"Input of "<<argv[i]<<"\n";
		cout<<"template image is: "<<
						recognitionEngine->getCurrentMove()->getMoveSeals().at(i-1)->getName()<<"\n";

		temp = cvLoadImage(argv[i], CV_LOAD_IMAGE_GRAYSCALE);
		img = cvCreateImage(cvSize(temp->width, temp->height), IPL_DEPTH_8U, 1);
		res = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_32F, img->nChannels);

		cvConvertScale(temp, img);

		/////////////////////////////////////
		//Processing phase:
		cout<<"Starting process\n";
		recognitionEngine->process(img, res);
		debugPrint("process ended\n");
		///////////////////////////////////

		//Now there is the template creator... use that instead
		//	if(CREATE_TEMPLATE){
		//		cout<<"creating template\n";
		//		cvSaveImage(TEMPLATE_NAME, res);
		//	}

		cvShowImage(INPUT_WIN, res);
		cvShowImage(TEMPLATE_WIN,
				recognitionEngine->getCurrentMove()->getMoveSeals().at(i-1)->getTemplateImage());
		cvWaitKey(5000);

		debugPrint("evaluation: \n");
		cout<<"score is: "<<recognitionEngine->evaluate(res, i-1)<<"\n";

		cvShowImage(INPUT_WIN, res);
		cvShowImage(TEMPLATE_WIN,
				recognitionEngine->getCurrentMove()->getMoveSeals().at(i-1)->getTemplateImage());
		cvWaitKey(10000);


	}

	debugPrint("out of cycle\n");

	cvReleaseImage(&img);
	cvReleaseImage(&res);
	cvReleaseImage(&temp);
	cvDestroyAllWindows();

	debugPrint("before return\n");
	return 0;
}


void initEngine(){

	//equalization
	recognitionEngine->addModule(new HistogramEM()); //<- remember to uncomment

	//Sobel
	//	recognitionEngine->addModule(new SobelEM(7, 2, 2));
	recognitionEngine->addModule(new SobelEM(CV_SCHARR, 1, 0)); //<-
	//	recognitionEngine->addModule(new SobelEM(CV_SCHARR, 0, 1));

	//Canny
	//	recognitionEngine->addModule(new CannyEM(5, 200, 40));

	//blurring
	recognitionEngine->addModule(new BlurEM(CV_GAUSSIAN,3,3)); //<-(?)

	//Laplacian
	recognitionEngine->addModule(new LaplacianEM(7)); //<-

	//contours extraction
	//	recognitionEngine->addModule(new ContoursFinderEM(120));
}
