#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
ofSetCircleResolution(100);
    // setup gui from ofxGui
    _gui.setup("Gui");
    _gui.setPosition(ofGetWidth()/2 , 0);

    // setup ofxOSSIA parameters to be add on the Gui
    ossia.setup();

    c.setup(ossia.getRootNode());

    for (int i=0 ; i<10 ; i++){
        InteractiveCircle c;
        c.setup(ossia.getRootNode());
        circles.push_back(c);
    }

    _gui.add (ossia.getRootNode());
    _gui.minimizeAll();

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    c.draw();

    for (int i=0 ; i<circles.size() ; i++){
        circles[i].draw();
    }


    //draw gui
    _gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
