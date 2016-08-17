#pragma once
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
namespace oscpack
{
    // in general NetworkInitializer is only used internally, but if you're
    // application creates multiple sockets from different threads at runtime you
    // should instantiate one of these in main just to make sure the networking
    // layer is initialized.
    class NetworkInitializer {
    public:
        NetworkInitializer() {}
        ~NetworkInitializer() {}
    };

    // return ip address of host name in host byte order
    inline unsigned long GetHostByName(const char *name)
    {
        unsigned long result = 0;

        struct hostent *h = gethostbyname(name);
        if (h) {
            struct in_addr a;
            std::memcpy(&a, h->h_addr_list[0], h->h_length);
            result = ntohl(a.s_addr);
        }

        return result;
    }
}