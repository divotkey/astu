// Copyright (c) 2020-2022 Roman Divotkey. All rights reserved.
//
// This file is subject to the terms and conditions defined in file 'LICENSE',
// which is part of this source code package. See 'AUTHORS' file for a list
// of contributors.

// Local includes
#include "Util/Buffer.h"

// C++ Standard Library includes
#include <cassert>
#include <cstring>
#include <algorithm>
#include <limits>

using namespace std;

namespace astu {

    Buffer::Buffer(size_t bufferSize)
    {
        data = make_unique<unsigned char[]>(bufferSize);
        pEnd = data.get() + bufferSize;
        Clear();
    }

    Buffer::~Buffer() {
        // Intentionally left empty.
    }

    void Buffer::Clear()
    {
        pCur = data.get();
        pLimit = pEnd;
    }

    void Buffer::Flip()
    {
        pLimit = pCur;
        pCur = data.get();
    }

    void Buffer::Rewind()
    {
        pCur = data.get();
    }

    void Buffer::SetLimit(size_t inLimit)
    {
        if (inLimit > GetCapacity()) {
            throw std::logic_error("Unable to set buffer limit, limit exceeds capacity");
        }
        pCur = data.get();
        pLimit = pCur + inLimit;
    }

    size_t Buffer::GetRemaining() const
    {
        assert(pLimit >= pCur);
        return pLimit - pCur;
    }

    size_t Buffer::Read(unsigned char *dst, size_t cnt) const
    {
        size_t n = std::min(GetRemaining(), cnt);
        memcpy(dst, pCur, n);
        pCur += n;
        return n;
    }

    void Buffer::Write(const unsigned char *src, size_t cnt)
    {
        if (GetRemaining() < cnt) {
            throw logic_error("Buffer overflow, request to write " + to_string(cnt) + " bytes, but only " +
                              to_string(GetRemaining()) + "bytes are available");
        }
        assert(GetRemaining() >= cnt);
        memcpy(pCur, src, cnt);
        pCur += cnt;
    }

    void Buffer::SkipBytes(size_t cnt) const
    {
        size_t n = std::min(GetRemaining(), cnt);
        pCur += n;
    }

    float Buffer::ReadFloat() const
    {
        if (GetRemaining() < sizeof(float)) {
            throw std::logic_error(
                    "Buffer underrun, unable to read float value, only " + to_string(GetRemaining()) + " bytes left");
        }

        return Read<float>();
    }

    double Buffer::ReadDouble() const
    {
        if (GetRemaining() < sizeof(double)) {
            throw std::logic_error(
                    "Buffer underrun, unable to read double value, only " + to_string(GetRemaining()) + " bytes left");
        }

        return Read<double>();
    }

    int16_t Buffer::ReadInt16() const
    {
        if (GetRemaining() < sizeof(int16_t)) {
            throw std::logic_error(
                    "Buffer underrun, unable to read 16bit integer value, only " + to_string(GetRemaining()) + " bytes left");
        }

        return Read<int16_t>();
    }

    int32_t Buffer::ReadInt32() const
    {
        if (GetRemaining() < sizeof(int16_t)) {
            throw std::logic_error(
                    "Buffer underrun, unable to read 16bit integer value, only " + to_string(GetRemaining()) + " bytes left");
        }

        return Read<int32_t>();
    }

    int64_t Buffer::ReadInt64() const
    {
        if (GetRemaining() < sizeof(int64_t)) {
            throw std::logic_error(
                    "Buffer underrun, unable to read 64bit integer value, only " + to_string(GetRemaining()) + " bytes left");
        }

        return Read<int64_t>();
    }

    std::string Buffer::ReadString() const
    {
        string result;
        ReadString(result);
        return result;
    }

    const std::string &Buffer::ReadString(string &outString) const
    {
        if (GetRemaining() < 1) {
            throw std::logic_error(
                    "Buffer underrun, unable to read string value, only " + to_string(GetRemaining()) + " bytes left");
        }
        unsigned char n = ReadByteFast();
        if (GetRemaining() < n) {
            throw std::logic_error(
                    "Buffer underrun, unable to read string value with " + to_string(n) + " characters), only " +
                    to_string(GetRemaining()) + " bytes left");
        }

        outString.clear();
        for (int i = 0; i < n; ++i) {
            outString.push_back(ReadByteFast());
        }
        return outString;
    }

    unsigned char Buffer::ReadByte() const
    {
        if (GetRemaining() < sizeof(unsigned char)) {
            throw std::logic_error(
                    "Buffer underrun, unable to read byte value, only " + to_string(GetRemaining()) + " bytes left");
        }

        return Read<unsigned char>();
    }

    unsigned char Buffer::ReadByteFast() const
    {
        assert(GetRemaining() >= sizeof(unsigned char));
        return Read<unsigned char>();
    }

    void Buffer::WriteByte(unsigned char value)
    {
        if (GetRemaining() < sizeof(unsigned char)) {
            throw std::logic_error(
                    "Buffer overflow, unable to write unsigned char value, only " + to_string(GetRemaining()) +
                    " bytes left");
        }
        Write<unsigned char>(value);
    }

    void Buffer::WriteByteFast(unsigned char value)
    {
        assert(GetRemaining() >= sizeof(unsigned char));
        Write<unsigned char>(value);
    }

    void Buffer::WriteFloat(float value)
    {
        if (GetRemaining() < sizeof(float)) {
            throw std::logic_error(
                    "Buffer overflow, unable to write float value, only " + to_string(GetRemaining()) +
                    " bytes left");
        }

        Write<float>(value);
    }

    void Buffer::WriteDouble(double value)
    {
        if (GetRemaining() < sizeof(double)) {
            throw std::logic_error(
                    "Buffer overflow, unable to write double value, only " + to_string(GetRemaining()) +
                    " bytes left");
        }

        Write<double>(value);
    }

    void Buffer::WriteInt16(int16_t value)
    {
        if (GetRemaining() < sizeof(int16_t)) {
            throw std::logic_error(
                    "Buffer overflow, unable to write 16 bit integer value, only " + to_string(GetRemaining()) +
                    " bytes left");
        }

        Write<int16_t>(value);
    }

    void Buffer::WriteInt32(int32_t value)
    {
        if (GetRemaining() < sizeof(int32_t)) {
            throw std::logic_error(
                    "Buffer overflow, unable to write 32 bit integer value, only " + to_string(GetRemaining()) +
                    " bytes left");
        }

        Write<int32_t>(value);
    }

    void Buffer::WriteInt64(int64_t value)
    {
        if (GetRemaining() < sizeof(int64_t)) {
            throw std::logic_error(
                    "Buffer overflow, unable to write 64 bit integer value, only " + to_string(GetRemaining()) +
                    " bytes left");
        }

        Write<int64_t>(value);
    }

    void Buffer::WriteString(const string &s)
    {
        if (s.size() > std::numeric_limits<unsigned char>::max()) {
            throw std::invalid_argument("Unable to write string to buffer, string exceeds maximum length of " +
                                        to_string(numeric_limits<unsigned char>::max()) + " characters");
        }
        if (GetRemaining() < sizeof(unsigned char) + s.size()) {
            throw std::logic_error(
                    "Buffer overflow, unable to write string value, only" + to_string(GetRemaining()) +
                    " bytes left");
        }
        WriteByteFast(static_cast<unsigned char>(s.size()));
        Write(reinterpret_cast<const unsigned char*>(s.data()), s.size());
    }

    unsigned char *Buffer::GetData()
    {
        return data.get();
    }

    const unsigned char *Buffer::GetData() const
    {
        return data.get();
    }

    unsigned char *Buffer::GetCurrentData()
    {
        return pCur;
    }

    const unsigned char *Buffer::GetCurrentData() const
    {
        return pCur;
    }


} // end of namespace