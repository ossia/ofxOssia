
//  InteractiveCircle.cpp
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 21/02/2017.
//
//

#include "InteractiveCircle.h"

void InteractiveCircle::setup(ossia::ParameterGroup _parent_node){
    
    _circleParams.setup(_parent_node, "circle");
    
    _sizeParams.setup(_circleParams, "sizeParams");
    _radius.setup(_sizeParams,"radius",ofRandomf()*99+1,1.,100.);
    _position.setup(_sizeParams,
                    "position",
                    ofVec2f(ofRandomWidth(), ofRandomHeight()),
                    ofVec2f(0., 0.), // Min
                    ofVec2f(ofGetWidth(), ofGetHeight())); // Max
    
    _colorParams.setup(_circleParams, "colorParams");
    _color.setup(_colorParams,
                 "color",
                 ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 255.),
                 ofColor(0., 0., 0., 0.),
                 ofColor(255., 255., 255., 255.));
    _fill.setup(_colorParams,"fill",false);

}

void InteractiveCircle::update(){
    
}

void InteractiveCircle::draw(){
    if(!_fill)
        ofNoFill();
    else
        ofFill();
    
    ofSetColor(_color.get());
    ofDrawCircle(_position.get(),_radius.get());
}
