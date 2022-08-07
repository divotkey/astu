/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stdint.h>
#include <string>
#include <iostream>

namespace astu {

    class IInetSocketAddress {
    public:

        /** Virtual destructor. */
        virtual ~IInetSocketAddress() {}

        /**
         * Returns the port of this socket address.
         *
         * @return the port number of this address
         */
        virtual uint16_t GetPort() const = 0;

        /**
         * Returns the socket address as string.
         *
         * @return the socket address as string
         */
        virtual const std::string& GetAddressString() const = 0;

        /**
         * Prints a string representation to the specified output stream.
         *
         * @param os    the output stream
         */
        virtual void PrintToStream(std::ostream &os) const = 0;

        /**
         * Requires for use as key in maps etc.
         *
         * @param rhs the right hand side for this operator
         * @return 'true' if this address is considers "less" than the rhs
         */
        virtual bool operator< (const IInetSocketAddress& rhs) const = 0;
    };

    inline std::ostream& operator<<(std::ostream& os, const IInetSocketAddress &addr) {
        addr.PrintToStream(os);
        return os;
    }

} // end of namespace