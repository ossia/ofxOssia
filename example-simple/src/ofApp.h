#pragma once

#include "ofMain.h"
#include "ofxOssia.hpp"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    //ofxOSSIA class
    Network _network;
    ossia::Parameter<float> _radius;
    ossia::Parameter<ofVec2f> _position;
    ossia::Parameter<bool> _fill;
    ossia::Parameter<ofColor> _color;

    //ofxGui
    ofxPanel            _gui;
};
