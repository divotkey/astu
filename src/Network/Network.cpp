/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Network/Network.h"
#include "NetworkImpl.h"
#include "Socket.h"

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

    std::unique_ptr<ISocket> Network::CreateUdpSocket(uint16_t port)
    {
        return nwImpl->CreateUdpSocket(port);
    }

    std::unique_ptr<ISocket> Network::CreateUdpSocket(const string &host, uint16_t port)
    {
        return nwImpl->CreateUdpSocket(host, port);
    }

    std::unique_ptr<IInetSocketAddress> Network::CreateUdpSocketAddress(const string &host, uint16_t port)
    {
        return nwImpl->CreateUdpSocketAddress(host, port);
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

} // end of namespace
