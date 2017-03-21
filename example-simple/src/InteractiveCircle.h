//
//  InteractiveCircle.h
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 21/02/2017.
//
//

#pragma once
#include "ofMain.h"
#include "ofxOssia.h"

class InteractiveCircle {
    
public :
    
//    ~InteractiveCircle(){
//        _colorParams.clearChild();
//        _sizeParams.clearChild();
//        _circleParams.clearChild();
//    }
    
    void setup(ossia::ParameterGroup _parent_node);
    void update();
    void draw();
    
    inline ossia::ParameterGroup & getCircleParams() {
        return _circleParams;
    }
    
private:
    ossia::Parameter<float> _radius;
    ossia::Parameter<ofVec2f> _position;
    ossia::Parameter<bool> _fill;
    ossia::Parameter<ofColor> _color;
    
    ossia::ParameterGroup _colorParams;
    ossia::ParameterGroup _sizeParams;
    ossia::ParameterGroup _circleParams;
};
