/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Network/IUdpSocket.h"

namespace astu {

    /**
     * Operating system specific implementation of IUdpSocket.
     */
    class UdpSocket : public IUdpSocket {
    public:

        /**
         * Constructor.
         */
        UdpSocket();

        /**
         * Destructor.
         */
        ~UdpSocket();

    private:
    };

} // namespace astu