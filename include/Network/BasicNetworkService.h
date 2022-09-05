/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/NetworkService.h"
#include "Service/TimeService.h"
#include "Network/Network.h"

namespace astu {

    /**
     * Implements the NetworkService class using the network OS abstraction layer.
     *
     * @ingroup network_group
     */
    class BasicNetworkService
        : public virtual Service
        , public NetworkService
    {
    public:

        BasicNetworkService();

        // Inherited via NetworkService
        int CreateAddressHandle(const std::string &host, uint16_t port) override;
        bool HasAddressHandle(const std::string &host, uint16_t port) const override;
        int GetAddressHandle(const std::string &host, uint16_t port) override;
        int GetOrCreateAddressHandle(const std::string &host, uint16_t port) override;
        std::string GetAddressString(int hAddr) const override;
        std::unique_ptr<Socket> CreateUdpSocket(uint16_t port) override;

    private:
        /** The actual OS independent implementation. */
        Network network;
    };

} // end of namespace
