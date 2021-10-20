#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    address  = "/tmp/ofxmodelrunner";
    socketAddr = "ipc://" + address;
    model.setup(socketAddr);


}
//--------------------------------------------------------------
void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    
    if( key >= 'a' && key <= 'z'){
        currentInput += key;
    } else if( key >= 'A' && key <= 'Z'){
        std::string c(1, key);
        currentInput += ofToLower(c);        
    } else if (key == OF_KEY_RETURN){
        ofLogNotice() << currentInput;

        model.sendText(currentInput);

        std::string output;
        model.receiveText(output);

        ofLogNotice() << output;

        inputs.push_back(currentInput);
        outputs.push_back(output);
        currentInput.clear();
    }
}