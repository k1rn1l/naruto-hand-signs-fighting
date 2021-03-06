/*
 * HandSeal.cxx
 *
 *  Created on: Nov 19, 2010
 *      Author: michele
 */

#include <iostream>
#include <opencv/highgui.h>
#include "HandSeal.h"

HandSeal::HandSeal(string m, string jap, string nameKey, int i){
	month = m;
	japaneseName = jap;
	name = nameKey;
	ID = i;
}

HandSeal::HandSeal(const char* m, const char* jap, const char* nameKey, int i){
	month = string(m);
	japaneseName = string(jap);
	name = string(nameKey);
	ID = i;
}

void HandSeal::setImages(const char* templImagePath, const char* thumbImagePath){
	IplImage *temp;
	try{
		templateImagePath = string(templImagePath);
		thumbnailImagePath = string(thumbImagePath);
		if(templImagePath != NULL){
			templateImage= cvLoadImage(templImagePath, CV_LOAD_IMAGE_GRAYSCALE);
//			templateImage = cvCreateImage(cvSize(temp->width, temp->height),DEFAULT_INPUT_DEPTH,1);
//			cvConvertScale(temp, templateImage);
		}
		if(thumbImagePath != NULL){
			thumbnailImage = cvLoadImage(thumbImagePath, CV_LOAD_IMAGE_GRAYSCALE);
		}
//		cout<<"loaded images of: "<<name<<"\n";
	}catch(cv::Exception e){
		cout<< e.err <<"\n";
	}
}

void HandSeal::displayTemplateImage(int delay){
	cvNamedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
	cvShowImage(name.c_str(), templateImage);
	cvWaitKey(delay);
	cvDestroyWindow(name.c_str());
}
