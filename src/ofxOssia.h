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
    ofxOssia():_device{std::make_unique<ossia::net::local_protocol>(), "ossia"},
		_local_proto(_device.getProtocol()){};

    /**
     * default setup for ofxOssia. Create a root node with oscquery protocol on port 3456 for OSC and 5678 for WS
     **/
    void setup();

    /**
     * @brief setup make an OSCQuery client
     * @param serverName the server address
     * @param serverPortWS the server listening port
     * @param localPortOSC the local listening port
     */
    void setup(const std::string& serverName, int serverPortWS, int localPortOSC);

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

    ossia::ParameterGroup & getRootNode(){return _root_node;};

//    ossia::ParameterGroup & getNode(std::string & name);
//    ossia::Parameter & getNode(std::string & name);


private:

    ossia::ParameterGroup _root_node;
    ossia::net::generic_device _device;
    ossia::net::protocol_base& _local_proto;

};
