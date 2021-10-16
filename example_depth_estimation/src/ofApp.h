#pragma once

#include "ofMain.h"
#include "ofxModelRunner.h"


struct image_desc {
	int w;
	int h;
	int c;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();



		int camWidth;
		int camHeight;

		ofVideoGrabber grabber;

    	ofImage inputImg, outputImg;
		ImageModelRunner model;

		image_desc input_desc { 256, 256, 3 };
		image_desc output_desc{ 256, 256, 1 };

		std::string address  = "/tmp/ofxmodelrunner";
		std::string socketAddr = "ipc://" + address;
};
