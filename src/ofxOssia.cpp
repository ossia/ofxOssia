//
//  ofxOssia.cpp
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 21/02/2017.
//  Updated to using SafeC++ implementation by Pascal Baltazar in 03/2018
//
//

#include "ofxOssia.h"


void ofxOssia::setup()
{
    _device.setup(default_device_name, 3456, 5678);
    _root_node.setup(_device.get_root_node(), default_device_name);

}

void ofxOssia::setup(std::string localname,
                     int localportOSC, int localPortWS)
{

    // declare a distant program as an OSCQuery device
    _device.setup(localname, localportOSC, localPortWS);
    _root_node.setup(_device.get_root_node(), localname);
}



