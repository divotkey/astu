/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <stddef.h>
#include <cassert>
#include <memory>

namespace astu {

    class Buffer {
    public:

        /**
         * Constructor.
         *
         * @param bufferSize the size of the byte buffer
         */
        Buffer(size_t bufferSize);

        /**
         * Destructor.
         */
        virtual ~Buffer();

        /**
         * Clears the content of this buffer.
         */
        void Clear();

        /**
         * Returns the current limit of this buffer.
         *
         * @return the limit in bytes
         */
        size_t GetLimit() const {
            return pLimit - data.get();
        }

        /**
         * Sets the limit of this buffer to the specified amount and set the current position to zero.
         *
         * @param limit the limit of this buffer
         */
        void SetLimit(size_t limit);

        /**
         * Return the capacity of this buffer.
         *
         * @return the capacity in bytes
         */
        size_t GetCapacity() const {
            return pEnd - data.get();
        }

        /**
         * Writes a specific number of bytes to this buffer.
         *
         * @param src   the data source
         * @param cnt   the number of bytes
         * @throws std::logic_error in case the buffer has insufficient storage capacity
         */
        void Write(const unsigned char *src, size_t cnt);

        /**
         * Reads the specified number of bytes from this buffer.
         * If less than the requested bytes are available in this buffer, only the available bytes are copied.
         *
         * @param dst   the destination memory
         * @param cnt   the number of bytes
         * @return the actual number of bytes transferred
         */
        size_t Read(unsigned char *dst, size_t cnt) const;

        /**
         * Skips the specified number of bytes.
         * If less than the requested bytes are available in this buffer, only the available bytes are skipped.
         *
         * @param cnt
         */
        void SkipBytes(size_t cnt) const;

        /**
         * Reads an unsigned character (byte) value from this buffer.
         *
         * @return the byte value
         * @throws std::logic_error in case of a buffer underrun
         */
        unsigned char ReadByte() const;

        /**
         * Writes an unsigned character (byte) to this buffer.
         *
         * @param value the byte value
         * @throws std::logic_error in case of a buffer overflow
         */
        void WriteByte(unsigned char value);

        /**
         * Reads an unsigned character (byte) value from this buffer.
         * This function does not check for sufficient data in the buffer in release builds.
         *
         * @return the byte value
         */
        unsigned char ReadByteFast() const;

        /**
         * Writes an unsigned character (byte) to this buffer.
         * This function does not check for sufficient storage in the buffer in release builds.
         *
         * @param value the byte value
         */
        void WriteByteFast(unsigned char value);

        /**
         * Reads a float value from this buffer.
         *
         * @return the float value.
         * @throws std::logic_error in case of a buffer underrun
         */
        float ReadFloat() const;

        /**
         * Writes a float to this buffer.
         *
         * @param value the float value
         * @throws std::logic_error in case of a buffer overflow
         */
        void WriteFloat(float value);

        /**
         * Reads a double value from this buffer.
         *
         * @return the double value.
         * @throws std::logic_error in case of a buffer underrun
         */
        double ReadDouble() const;

        /**
         * Writes a double to this buffer.
         *
         * @param value the double value
         * @throws std::logic_error in case of a buffer overflow
         */
        void WriteDouble(double value);

        /**
         * Reads a 16 bit signed integer value from this buffer.
         *
         * @return the i6 bit integer value
         * @throws std::logic_error in case of a buffer underrun
         */
        int16_t ReadInt16() const;

        /**
         * Writes a 16 bit integer to this buffer.
         *
         * @param value the 16 bit integer value
         * @throws std::logic_error in case of a buffer overflow
         */
        void WriteInt16(int16_t value);

        /**
         * Reads a 32 bit signed integer value from this buffer.
         *
         * @return the 32 bit integer value
         * @throws std::logic_error in case of a buffer underrun
         */
        int32_t ReadInt32() const;

        /**
         * Writes a 32 bit integer to this buffer.
         *
         * @param value the 32 bit integer value
         * @throws std::logic_error in case of a buffer overflow
         */
        void WriteInt32(int32_t value);

        /**
         * Reads a 64 bit signed integer value from this buffer.
         *
         * @return the 64 bit integer value
         * @throws std::logic_error in case of a buffer underrun
         */
        int64_t ReadInt64() const;

        /**
         * Writes a 64 bit integer to this buffer.
         *
         * @param value the 64 bit integer value
         * @throws std::logic_error in case of a buffer overflow
         */
        void WriteInt64(int64_t value);

        /**
         * Reads a string from this buffer.
         *
         * @return the string
         * @throws std::logic_error in case of a buffer underrun
         */
        std::string ReadString() const;

        /**
         * Reads a string from this buffer.
         *
         * @param outString receives the string data
         * @return a reference to the result string
         * @throws std::logic_error in case of a buffer underrun
         */
        const std::string & ReadString(std::string& outString) const;

        /**
         * Writes a string fo this buffer.
         *
         * @param s the string
         * @throws std::logic_error in case of a buffer overflow
         */
        void WriteString(const std::string& s);

        /**
         * Flips this buffer.
         *
         * The limit is set to the current position and then the position is set to zero. Use this method before change
         * from 'writing to a buffer' to 'reading from a buffer'.
         */
        void Flip();

        /**
         * Rewinds this buffer.
         * The position is set to zero.
         */
        void Rewind();

        /**
         *
         * @return
         */
        size_t GetRemaining() const;

        /**
         * Provides access to the raw memory storage of this buffer.
         *
         * @return the storage memory of this buffer.
         */
        unsigned char* GetData();

        /**
         * Provides access to the raw memory storage of this buffer.
         *
         * @return the storage memory of this buffer.
         */
        const unsigned char *GetData() const;

        /**
         * Returns access to the current position of the raw memory storage.
         *
         * @return the current position within the raw memory storage
         */
        unsigned char* GetCurrentData();

        /**
         * Returns access to the current position of the raw memory storage.
         *
         * @return the current position within the raw memory storage
         */
        const unsigned char* GetCurrentData() const;

    private:
        /** The actual storage memory of this buffer. */
        std::unique_ptr<unsigned char[]> data;

        /** Points to the first byte after the data of this buffer. */
        unsigned char *pEnd;

        /** The current reading position. */
        mutable unsigned char *pCur;

        /** The limit pointer, used to define the available bytes for reading/writing. */
        unsigned char *pLimit;

        template <typename T>
        T Read() const {
            assert(GetRemaining() >= sizeof(T));
            T result;
            Read(reinterpret_cast<unsigned char*>(&result), sizeof(T));
            return result;
        }

        template <typename T>
        void Write(const T &value) {
            assert(GetRemaining() >= sizeof(T));
            Write(reinterpret_cast<const unsigned char*>(&value), sizeof(T));
        }

    };

} // end of namespace
