/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "IInetSocketAddress.h"
#include "Util/Buffer.h"

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
        void SendTo(std::vector<unsigned char> buf, int hAddr) {
            SendTo(buf.data(), buf.size(), hAddr);
        }

        void SendTo(const astu::Buffer & buffer, int hAddr) {
            SendTo(buffer.GetData(), buffer.GetRemaining(), hAddr);
        }

        /**
         * Writes the content of the specified buffer to this socket.
         *
         * @param buf   the buffer data
         * @param lng   the number of bytes to send
         * @param hAddr handle to the target address
         */
        virtual void SendTo(const unsigned char *buf, size_t lng, int hAddr) = 0;

        /**
         * Receives data from this socket.
         *
         * @param buffer receives the data
         * @return the address handle of the sender
         */
        virtual int Receive(Buffer &buffer) = 0;

        //virtual void Read(unsigned char* buf, size_t, )
    };

} // end of namespace
