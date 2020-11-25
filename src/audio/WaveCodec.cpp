/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <limits>
#include <string>
#include "WaveCodec.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// RiffChunk Base Class
    /////////////////////////////////////////////////

    RiffChunk::RiffChunk(const char* code, unsigned int size)
        : fourcc{code[0], code[1], code[2], code[3]}
        , dataSize(size) 
    {
        // Intentionally left empty.
    }

    void RiffChunk::ReadFourCC(std::istream& is)
    {
        char code[4];
        is.read(code, 4);
        ValidateStream(is);

        if (!IsFourCC(code)) {
            throw std::runtime_error("Four-character code mismatch");
        }
    }

    const char* RiffChunk::GetFourCC() const
    {
        return fourcc;
    }

    void RiffChunk::Write(std::ostream& os) const
    {
        os.write(fourcc, 4);
        Write4ByteUInt(os, static_cast<uint32_t>(dataSize));
        WriteBody(os);
    }

    void RiffChunk::ReadWithoutFourCC(std::istream& is)
    {
        dataSize = Read4ByteUInt(is);
        ValidateStream(is);
        ReadBody(is);
    }

    void RiffChunk::Read(std::istream& is)    
    {
        ReadFourCC(is);
        ReadWithoutFourCC(is);
    }

    unsigned int RiffChunk::GetDataSize() const
    {
        return dataSize;
    }

    unsigned int RiffChunk::GetSize() const
    {
        return GetDataSize() + 8;
    }

    void RiffChunk::Write2ByteUInt(std::ostream& os, uint16_t value) const
    {
        os.write(reinterpret_cast<const char*>(&value), sizeof(uint16_t));
    }

    void RiffChunk::Write4ByteUInt(std::ostream& os, uint32_t value) const
    {
        os.write(reinterpret_cast<const char*>(&value), sizeof(uint32_t));
    }

    unsigned int RiffChunk::Read2ByteUInt(std::istream& is) const
    {
        uint16_t value;
        is.read(reinterpret_cast<char*>(&value), sizeof(uint16_t));

        return value;
    }

    unsigned int RiffChunk::Read4ByteUInt(std::istream& is) const
    {
        uint32_t value;
        is.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
        return value;
    }

    void RiffChunk::ValidateStream(std::istream& is) const
    {
        if (!is.good()) {
            throw std::runtime_error("Failed reading input stream");
        }
    }

    bool RiffChunk::IsFourCC(const char *code) const
    {
        if (code == nullptr) {
            return false;
        }

        return fourcc[0] == code[0] 
            && fourcc[1] == code[1] 
            && fourcc[2] == code[2] 
            && fourcc[3] == code[3];
    }

    /////////////////////////////////////////////////
    /////// WaveChunk
    /////////////////////////////////////////////////

    WaveChunk::WaveChunk(unsigned int subChunkSize)
        : RiffChunk("RIFF", subChunkSize + 4)
        , type{'W', 'A', 'V', 'E'}
    {
        // Intentionally left empty.
    }

    void WaveChunk::WriteBody(std::ostream& os) const
    {
        os.write(type, 4);
    }

    void WaveChunk::ReadBody(std::istream& is)
    {
        is.read(type, 4);
        ValidateStream(is);

        if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E') {
            throw std::runtime_error("Invalid wave chunk header");
        }
    }

    /////////////////////////////////////////////////
    /////// FormatChunk
    /////////////////////////////////////////////////

    const unsigned int FormatChunk::kDataSize = 16;

    FormatChunk::FormatChunk()
        : RiffChunk("fmt ", kDataSize)
        , audioFormat(AudioFormat::PCM)
        , numberOfChannels(1)
        , sampleRate(44100)
        , bitsPerSample(16)
    {
        // Intentionally left empty.
    }

    unsigned int FormatChunk::GetNumberOfChannels() const
    {
        return numberOfChannels;
    }

    void FormatChunk::SetNumberOfChannels(unsigned int ch) 
    {
        if (ch == 0 || ch > std::numeric_limits<uint16_t>::max()) {
            throw std::domain_error("Invalid number of channels, got " + std::to_string(ch));
        }
        numberOfChannels = ch;
    }

    void FormatChunk::WriteBody(std::ostream& os) const
    {
        if (GetDataSize() != 16) {
            std::cout << "...:::FUCK:::..." << std::endl;
        }
        Write2ByteUInt(os, static_cast<uint16_t>(audioFormat));
        Write2ByteUInt(os, static_cast<uint16_t>(numberOfChannels));
        Write4ByteUInt(os, static_cast<uint32_t>(sampleRate));
        Write4ByteUInt(os, static_cast<uint32_t>(byteRate));
        Write2ByteUInt(os, static_cast<uint16_t>(blockAlign));
        Write2ByteUInt(os, static_cast<uint16_t>(bitsPerSample));
    }

    void FormatChunk::ReadBody(std::istream& is)
    {
        audioFormat = static_cast<AudioFormat>(Read2ByteUInt(is));
        numberOfChannels = Read2ByteUInt(is);
        sampleRate = Read4ByteUInt(is);
        byteRate = Read4ByteUInt(is);
        blockAlign = Read2ByteUInt(is);
        bitsPerSample = Read2ByteUInt(is);
        ValidateStream(is);
        if (GetDataSize() > kDataSize) {
            is.seekg(GetDataSize() - kDataSize, is.cur);
        }
    }

    auto FormatChunk::GetAudioFormat() const -> AudioFormat
    {
        return audioFormat;
    }

    void FormatChunk::SetAudioFormat(AudioFormat format)
    {
        audioFormat = format;
    }

    unsigned int FormatChunk::GetSampleRate() const
    {
        return sampleRate;
    }

    void FormatChunk::SetSampleRate(unsigned int rate)
    {
        sampleRate = rate;
    }

    unsigned int FormatChunk::GetBitsPerSample() const
    {
        return bitsPerSample;
    }

    void FormatChunk::SetBitsPerSamples(unsigned int numBits)
    {
        if (numBits == 0 || numBits > std::numeric_limits<uint16_t>::max()) {
            throw std::domain_error("Invalid number of bits per sample, got " + std::to_string(numBits));
        }

        bitsPerSample = numBits;
    }

    unsigned int FormatChunk::GetBlockAlign() const
    {
        return blockAlign;
    }

    void FormatChunk::SetBlockAlign(unsigned int numBytes)
    {
        if (numBytes == 0 || numBytes > std::numeric_limits<uint16_t>::max()) {
            throw std::domain_error("Invalid number bytes for block alignment, got " + std::to_string(numBytes));
        }
        blockAlign = numBytes;
    }

    void FormatChunk::UpdateBlockAlign()
    {
        blockAlign = GetNumberOfChannels() * ((GetBitsPerSample() + 7) / 8);
    }

    unsigned FormatChunk::GetByteRate() const
    {
        return byteRate;
    }

    void FormatChunk::SetByteRate(unsigned int bytesPerSecond)
    {
        byteRate = bytesPerSecond;
    }

    void FormatChunk::UpdateByteRate()
    {
        byteRate = GetSampleRate() * GetBlockAlign();
    }

    void FormatChunk::Update()
    {
        UpdateBlockAlign();
        UpdateByteRate();
    }

    /////////////////////////////////////////////////
    /////// DataChunk
    /////////////////////////////////////////////////

    DataChunk::DataChunk(unsigned int dataSize)
        : RiffChunk("data", dataSize)
    {
        // Intentionally left empty.
    }

    /////////////////////////////////////////////////
    /////// SkipChunk
    /////////////////////////////////////////////////

    SkipChunk::SkipChunk(const char* code)
        : RiffChunk(code != nullptr ? code : "????", 0)
    {
        // Intentionally left empty.
    }

    void SkipChunk::ReadFourCC(std::istream& is)
    {
        is.read(fourcc, 4);
        ValidateStream(is);
    }

    void SkipChunk::WriteBody(std::ostream& os) const
    {
        throw std::logic_error("SkipChunk must not be written to output stream.");
    }

    void SkipChunk::ReadBody(std::istream& is)
    {
        is.seekg(GetDataSize(), is.cur);
    }
}
