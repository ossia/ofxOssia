#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#if defined(Bool)
#undef Bool
#endif

#if defined(True)
#undef True
#endif

#if defined(False)
#undef False
#endif

#if defined(Status)
#undef Status
#endif

#if defined(None)
#undef None
#endif

#include <ossia/ossia.hpp>

class Network
{
public:
    Network(const std::string& localname,
            const std::string& remotename,
            const std::string& remoteip,
            int localport, int remoteport);

    // get the scene node
    ossia::net::node_base& getSceneNode();

private:
    ossia::net::generic_device _device;
    ossia::net::node_base* _localSceneNode{};
    std::thread _networkThread;
};

#endif // NETWORK_H
