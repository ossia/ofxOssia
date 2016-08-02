#include "network.hpp"

Network::Network(
        const std::string& localname,
        const std::string& remotename,
        const std::string& remoteip,
        int localport, int remoteport):
    _device{std::make_unique<ossia::net::local_protocol>(), localname}
{
    auto& local_proto = static_cast<ossia::net::local_protocol&>(_device.getProtocol());

    // declare a distant program as a Minuit device
    local_proto.exposeTo(
                std::make_unique<ossia::net::minuit_protocol>(
                    localname, remoteip, localport, remoteport));

    // add a node "scene"
    _localSceneNode = _device.getRootNode().createChild("scene");
}


ossia::net::node_base& Network::getSceneNode()
{
    return *_localSceneNode;
}

