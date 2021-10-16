#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    address  = "/tmp/ofxmodelrunner";
    socketAddr = "ipc://" + address;
    model.setup(socketAddr);

    camWidth = 640;  // try to grab at this size.
    camHeight = 480;

    //get back a list of devices.
    vector<ofVideoDevice> devices = grabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    grabber.setDeviceID(2);
    grabber.setup(camWidth, camHeight, true);

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetLastFrameTime()));
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

        inputImgRS = ofImage(inputImg);
        inputImgRS.resize(output_desc.w, output_desc.h);

        model.sendPixels(inputImg.getPixels());

        ofFloatPixels output;
        model.receivePixels(output);

        ofLogNotice() << output[0];

        outputImg.setFromPixels(output);


        uint64_t end = ofGetSystemTimeMicros();
        ofLogNotice() << end - srt;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    inputImgRS.draw(0, 0);
    outputImg.draw(inputImgRS.getWidth(), 0);
}
