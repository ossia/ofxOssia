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
    ofxOssia():_device{std::make_unique<ossia::net::local_protocol>(), "ossia"}{};
    
    void setup();
    void setup(const std::string& type,
               const std::string& localname,
               int localportOSC, int localPortWS);
    void setup(const std::string& type,
               const std::string& localname,
               const std::string& remotename,
               const std::string& remoteip,
               int localport, int remoteport);
    
    ossia::ParameterGroup & getRootNode(){return _root_node;};
    
private:
    
    ossia::ParameterGroup _root_node;
    ossia::net::generic_device _device;
    
};
