/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Network/Network.h"
#include "NetworkImpl.h"

using namespace std;

namespace astu {

    Network::Network()
    {
        nwImpl = make_unique<NetworkImpl>();
    }

    Network::~Network()
    {
        // Intentionally left empty.
    }

    std::unique_ptr<Socket> Network::CreateUdpSocket(uint16_t port)
    {
        return nwImpl->CreateUdpSocket(port);
    }

    std::unique_ptr<Socket> Network::CreateUdpSocket(const string &host, uint16_t port)
    {
        return nwImpl->CreateUdpSocket(host, port);
    }

    void Network::SetIpMode(IpMode mode)
    {
        nwImpl->SetIpMode(mode);
    }

    IpMode Network::GetIpMode() const
    {
        return nwImpl->GetIpMode();
    }

    int Network::CreateAddressHandle(const string &host, uint16_t port)
    {
        return nwImpl->CreateAddressHandle(host, port);
    }

    bool Network::HasAddressHandle(const string &host, uint16_t port) const
    {
        return nwImpl->HasAddressHandle(host, port);
    }

    int Network::GetAddressHandle(const string &host, uint16_t port)
    {
        return nwImpl->GetAddressHandle(host, port);
    }

    std::string Network::GetAddressString(int hAddr) const
    {
        return nwImpl->GetAddressString(hAddr);
    }

    int Network::GetOrCreateAddressHandle(const string &host, uint16_t port)
    {
        return nwImpl->GetOrCreateAddressHandle(host, port);
    }

} // end of namespace
