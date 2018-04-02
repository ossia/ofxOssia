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
    cout << "Setup default device \n";
    _device.setup(default_device_name, 3456, 5678);
    _root_node.setup(_device);

}

void ofxOssia::setup(std::string localname,
                     int localportOSC, int localPortWS)
{
    cout << "Setup device " <<  localname << " with ports OSC: " << localportOSC << " & WS: " << localPortWS << "\n";
    // declare a distant program as an OSCQuery device
    _device.setup(localname, localportOSC, localPortWS);
    _root_node.setup(_device);
}



