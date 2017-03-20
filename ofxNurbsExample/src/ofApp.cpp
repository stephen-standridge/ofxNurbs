#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth 		= 1280;	// try to grab at this size.
    camHeight 		= 720;
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    nurbs.setup(vidGrabber.getPixels(),10,10);
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    nurbs.updateImage(vidGrabber.getPixels());
}

//--------------------------------------------------------------
void ofApp::draw(){
    //vidGrabber.draw(20,20);
    
    nurbs.draw();
    /*
     ofNoFill();
     ofSetColor(0);
     ofRect((ofGetWidth()-camWidth)/2,(ofGetHeight()-camHeight)/2, camWidth,camHeight);
     */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'D' || key == 'd'){
        nurbs.toggleEditMode();
    }
    
    if(nurbs.getEditMode()){
        nurbs.keyPressed(key);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
