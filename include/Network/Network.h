/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "IUdpSocket.h"

// C++ Standard Library includes
#include <memory>

namespace astu {

    class Network {
    public:

        /** Constructor. */
        Network();

        /** Destructor. */
        ~Network();

        std::unique_ptr<IUdpSocket> CreateUdpSocket();

    private:

    };

} // end of namespace
