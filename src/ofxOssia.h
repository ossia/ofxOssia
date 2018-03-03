#pragma once
#undef Status
#undef Bool
#undef bool
#undef False
#undef status
#undef None
#include <ossia-cpp98.hpp>
#include "Parameter.h"

class ofxOssia {

public:
    ofxOssia():
        _device("ossia", 3456, 5678){
        _root_node.setup (_device.get_root_node(), "ossia");
    }

    /**
     * default setup for ofxOssia. Create a root node with oscquery protocol on port 3456 for OSC and 5678 for WS
     **/
    void setup();

    /**
     * run with type == "oscquery" then specify name and specific port
     **/
    void setup(string localname,
               int localportOSC, int localPortWS);


    ossia::ParameterGroup & get_root_node(){return _root_node;}
    opp::oscquery_server & get_device(){return _device;}

//    ossia::ParameterGroup & getNode(std::string & name);
//    ossia::Parameter & getNode(std::string & name);


private:

    ossia::ParameterGroup _root_node;
    opp::oscquery_server _device;

};
