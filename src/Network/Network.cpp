/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Network/Network.h"
#include "NetworkImpl.h"
#include "UdpSocket.h"

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

    std::unique_ptr<IUdpSocket> Network::CreateUdpSocket(uint16_t port)
    {
        return nwImpl->CreateUdpSocket(port);
    }

} // end of namespace
