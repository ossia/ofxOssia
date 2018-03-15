#pragma once

#include "ofMain.h"
#include "ofxOssia.h"
#include "ofxGui.h"

#include "InteractiveCircle.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);

private:
    
    ofxOssia ossia;

    vector<InteractiveCircle> circles;

    ofxPanel            _gui;
};
