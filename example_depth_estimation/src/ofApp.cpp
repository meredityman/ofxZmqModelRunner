#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    address  = "/tmp/ofxmodelrunner";
    socketAddr = "ipc://" + address;
    model.setup(socketAddr);

    camWidth = 640;  // try to grab at this size.
    camHeight = 480;


    grabber.setup(camWidth, camHeight, true);

}

//--------------------------------------------------------------
void ofApp::update(){

    grabber.update();

    if( grabber.isFrameNew()){
        uint64_t srt = ofGetSystemTimeMicros();

        ofPixels raw(grabber.getPixels());
        raw.resize(input_desc.w, input_desc.h, OF_INTERPOLATE_NEAREST_NEIGHBOR);
        ofFloatPixels input;
        input.allocate(raw.getWidth(), raw.getHeight(), raw.getPixelFormat());

        for(size_t i = 0; i < input.size(); i++){
            input[i] = (float)raw[i] / 255.0;
        }

        inputImg.setFromPixels(input);
        model.sendPixels(inputImg.getPixels());

        ofFloatPixels output;
        model.receivePixels(output);

        outputImg.setFromPixels(output);

        uint64_t end = ofGetSystemTimeMicros();
        ofLogNotice() << end - srt;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    inputImg.draw(0, 0);
    outputImg.draw(inputImg.getWidth(), 0);
}
