/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>

namespace astu {

    class IUdpSocket {
    public:

        virtual void Bind(int port);

    private:

    };

} // end of namespace
