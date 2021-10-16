#pragma once

#include "ofMain.h"
#include "ofxModelRunner.h"


#define SIZE_X 64
#define SIZE_Y 64
#define FORMAT OF_PIXELS_RGB
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


    	ofImage inputImg, outputImg;
		ImageModelRunner model;

		std::string address  = "/tmp/ofxmodelrunner";
		std::string socketAddr = "ipc://" + address;
};
