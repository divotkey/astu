/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Network/BasicNetworkService.h"

using namespace std;

namespace astu {

    BasicNetworkService::BasicNetworkService()
        : Service("Basic Network Service")
    {
        // Intentionally left empty.
    }

    int BasicNetworkService::CreateAddressHandle(const string &host, uint16_t port)
    {
        return network.CreateAddressHandle(host, port);
    }

    bool BasicNetworkService::HasAddressHandle(const string &host, uint16_t port) const
    {
        return network.HasAddressHandle(host, port);
    }

    int BasicNetworkService::GetAddressHandle(const string &host, uint16_t port)
    {
        return network.GetAddressHandle(host, port);
    }

    int BasicNetworkService::GetOrCreateAddressHandle(const string &host, uint16_t port)
    {
        return network.GetOrCreateAddressHandle(host, port);
    }

    unique_ptr <Socket> BasicNetworkService::CreateUdpSocket(uint16_t port)
    {
        return network.CreateUdpSocket(port);
    }

} // end of namespace