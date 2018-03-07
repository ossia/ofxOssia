#pragma once
#undef Status
#undef Bool
#undef bool
#undef False
#undef status
#undef None
#include <ossia-cpp98.hpp>
#include "Parameter.h"

#define default_device_name "ofxOssia"

class ofxOssia {

public:
    ofxOssia():
        _device(){
        _root_node.setup (_device.get_root_node(), default_device_name);
    }

    /**
     * default setup for ofxOssia. Create a root node with oscquery protocol on port 3456 for OSC and 5678 for WS
     **/
    void setup();

    /**
     * specify name and specific ports for OSC and WS
     **/
    void setup(std::string localname,
               int localportOSC, int localPortWS);


    ossia::ParameterGroup & get_root_node(){return _root_node;}
    opp::oscquery_server & get_device(){return _device;}

//    ossia::ParameterGroup & getNode(std::string & name);
//    ossia::Parameter & getNode(std::string & name);


private:

    ossia::ParameterGroup _root_node;
    opp::oscquery_server _device;

};
