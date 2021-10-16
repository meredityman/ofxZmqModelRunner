#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    address  = "/tmp/ofxmodelrunner";
    socketAddr = "ipc://" + address;
    model.setup(socketAddr);

    ofFloatPixels input;
    input.allocate(SIZE_X, SIZE_Y, FORMAT);

    for(size_t y=0; y < SIZE_Y; y++)
        for(size_t x=0; x < SIZE_X; x++)
            input.setColor(x, y, ofFloatColor((float)x/SIZE_X,(float)y/SIZE_Y, 0.0));
            

    inputImg.setFromPixels(input);

}
//--------------------------------------------------------------
void ofApp::update(){

    uint64_t srt = ofGetSystemTimeMicros();

    ofFloatPixels output;

    model.sendPixels(inputImg.getPixels());
    model.receivePixels(output);

    outputImg.setFromPixels(output);

    uint64_t end = ofGetSystemTimeMicros();
    ofLogNotice() << end - srt;
}

//--------------------------------------------------------------
void ofApp::draw(){
    inputImg.draw(0, 0);
    outputImg.draw(inputImg.getWidth(), 0);
}
