/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "IInetSocketAddress.h"

// C++ Standard Library includes
#include <string>
#include <vector>

namespace astu {

    /**
     * Interface for sockets.
     */
    class ISocket {
    public:

        /** Virtual destructor. */
        virtual ~ISocket() {}

        /**
         * Checks the status of this socket.
         */
        virtual void Poll() = 0;

        /**
         * Checks whether messages have arrived on this socket.
         *
         * @return `true` if this socket has messages to read
         */
        virtual bool IsReadyToRead() const = 0;

        /**
         * Writes the content of the specified buffer to this socket.
         *
         * @param buf   the buffer
         * @param addr  the target address
         */
        void SendTo(std::vector<unsigned char> buf, const IInetSocketAddress &addr) {
            SendTo(buf.data(), buf.size(), addr);
        }

        /**
         * Writes the content of the specified buffer to this socket.
         *
         * @param buf   the buffer data
         * @param lng   the number of bytes to send
         * @param addr  the target address
         */
        virtual void SendTo(unsigned char *buf, size_t lng, const IInetSocketAddress &addr) = 0;

        /**
         * Writes the content of the specified buffer to this socket.
         *
         * @param buf   the buffer data
         * @param lng   the number of bytes to send
         * @param hAddr handle to the target address
         */
        virtual void SendTo(unsigned char *buf, size_t lng, int hAddr) = 0;

        //virtual int CreateAddressHandle(const IInetSocketAddress& addr) = 0;
        virtual int CreateAddressHandle(const std::string & host, uint16_t port) = 0;
        virtual bool HasAddressHandle(int hAddr) const = 0;
        virtual const IInetSocketAddress& GetAddressFromHandle(int hAddr) const = 0;

        //virtual void Read(unsigned char* buf, size_t, )
    };

} // end of namespace
