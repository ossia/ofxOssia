#pragma once
#undef Status
#undef Bool
#undef bool
#undef False
#undef status
#undef None
#include <ossia/ossia.hpp>
#include "Parameter.h"

class ofxOssia {

public:
    ofxOssia():_device{std::make_unique<ossia::net::local_protocol>(), "ossia"}{
        _root_node.setup (_device.get_root_node(), "ossia");
    };

    /**
     * default setup for ofxOssia. Create a root node with oscquery protocol on port 3456 for OSC and 5678 for WS
     **/
    void setup();

    /**
     * run with type == "oscquery" then specify name and specific port
     **/
    void setup(const std::string& type,
               const std::string& localname,
               int localportOSC, int localPortWS);

    /**
     * run with type == "minuit" to setup with minuit protocol then specify name, ip and port
     **/
    void setup(const std::string& type,
               const std::string& localname,
               const std::string& remotename,
               const std::string& remoteip,
               int localport, int remoteport);

    ossia::ParameterGroup & get_root_node(){return _root_node;};

//    ossia::ParameterGroup & getNode(std::string & name);
//    ossia::Parameter & getNode(std::string & name);


private:

    ossia::ParameterGroup _root_node;
    ossia::net::generic_device _device;

};
