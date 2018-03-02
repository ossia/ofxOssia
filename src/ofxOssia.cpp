//
//  ofxOssia.cpp
//  ofxOSSIA
//
//  Created by Thomas Pachoud on 21/02/2017.
//  Updated to using SafeC++ implementation by Pascal Baltazar in 03/2018
//
//

#include "ofxOssia.h"
//#include "ossia/network/oscquery/oscquery_server.hpp"

void ofxOssia::setup(){
    _device("ossia", 3456, 5678);
}

void ofxOssia::setup(const std::string& localname,
                     int localportOSC, int localPortWS){
    // auto& local_proto = static_cast<ossia::net::local_protocol&>(_device.get_protocol());

    // declare a distant program as an OSCQuery device
    // local_proto.expose_to (std::make_unique<ossia::oscquery::oscquery_server_protocol> (localportOSC, localPortWS));

    _device(localname, localportOSC, localPortWS);

    opp::node _root_node = _device.get_root_node().create_node(localname);

    //ossia_node_t root = ossia_device_get_root_node(localname);
    
    //_root_node.setup (_device.get_root_node(), localname);
}



