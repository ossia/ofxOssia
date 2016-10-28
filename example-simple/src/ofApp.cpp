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
    _gui.add(_radius.setup(_network.getSceneNode(),"radius",10.,1.,100.));
    _gui.add(
        _position.setup(
            _network.getSceneNode(),
            "position", 
            ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2), 
            ofVec2f(0., 0.), // Min
            ofVec2f(ofGetWidth(), ofGetHeight()))); // Max

     
    _gui.add(
        _color.setup(
            _network.getSceneNode(),
            "color", 
            ofColor(123., 255., 17., 255.),
            ofColor(0., 0., 0., 0.),
            ofColor(255., 255., 255., 255.)));
        
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
        
    ofSetColor(_color.get());
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
