#include "network.hpp"

Network::Network(
        const std::string& localname,
        const std::string& remotename,
        const std::string& remoteip,
        int localport, int remoteport)
{
    // declare this program "B" as Local device
    _localProtocol = Local::create();
    _localDevice = Device::create(_localProtocol, localname);

    // add a node "scene"
    _localSceneNode = *(_localDevice->emplace(_localDevice->children().cend(), "scene"));

    _remoteProtocol = Minuit::create(remoteip, localport, remoteport);
    _remoteDevice = Device::create(_remoteProtocol, remotename);
}


std::shared_ptr<Node> Network::getSceneNode()
{
    return _localSceneNode;
}

