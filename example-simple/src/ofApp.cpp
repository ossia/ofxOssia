#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetCircleResolution(25);
    // setup gui from ofxGui
    _gui.setup("Gui");
    _gui.setPosition(0 , 0);

    // setup ofxOSSIA parameters to be add on the Gui
    ossia.setup();
    
    ossia.getRootNode();
    
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
    if (key == '+'){
        for (int i=0 ; i<10 ; i++){
            InteractiveCircle c;
            c.setup(ossia.getRootNode());
            circles.push_back(c);
        }
    }
    else if (key == '-'){
        for (int i=0 ; i<5 && circles.size()>1 ; i++){
            circles.pop_back();
        }
    }
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
