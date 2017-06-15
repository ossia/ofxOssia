//
//  ofxOssia.cpp
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 21/02/2017.
//
//

#include "ofxOssia.h"
#include "ossia/network/oscquery/oscquery_server.hpp"

void ofxOssia::setup(){
    setup("OSCQuery", "ofxOssiaDevice", 3456, 5678);
}

void ofxOssia::setup(const std::string& type,
                     const std::string& localname,
                     int localportOSC, int localPortWS){
    auto& local_proto = static_cast<ossia::net::local_protocol&>(_device.get_protocol());

    // declare a distant program as an OSCQuery device
    local_proto.expose_to (std::make_unique<ossia::oscquery::oscquery_server_protocol> (localportOSC, localPortWS));

    _root_node.setup (_device.get_root_node(), localname);
}

void ofxOssia::setup(const std::string& type,
                     const std::string& localname,
                     const std::string& remotename,
                     const std::string& remoteip,
                     int localport, int remoteport){

    auto& local_proto = static_cast<ossia::net::local_protocol&>(_device.get_protocol());

    // declare a distant program as a Minuit device
    local_proto.expose_to(std::make_unique<ossia::net::minuit_protocol>(localname, remoteip, localport, remoteport));

    _root_node.setup(_device.get_root_node(), localname);
}

