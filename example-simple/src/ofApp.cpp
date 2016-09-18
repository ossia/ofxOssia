#include "ofApp.h"
//--------------------------------------------------------------
ofApp::ofApp():
     _network("ofExample","i-score","127.0.0.1",13579, 9998)
   // initialized network with local device 'ofExample' and remote device 'i-score' at localhost
{

}

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    // setup gui from ofxGui
    _gui.setup("Gui");
    _gui.setPosition(ofGetWidth()/2 , 0);

    // setup ofxOSSIA parameters to be add on the Gui
    _gui.add(_radius.setup(_network.getSceneNode(),"radius",10.,1.,100.));
    _gui.add(_position.setup(_network.getSceneNode(),"position",ofVec2f(ofGetWidth()/2,ofGetHeight()/2),ofVec2f(0.,0.),ofVec2f(ofGetWidth(),ofGetHeight())));
    _gui.add(_fill.setup(_network.getSceneNode(),"fill",false));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    if(!_fill)
        ofNoFill();
    else
        ofFill();
        
    ofDrawCircle(_position.get(),_radius.get());

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
