/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 * 
 *----------------------------------------------------------------------------
 * Meanings of ASTU: Slightly melted butter (Hinduism / Religious Law), 
 * Let it be / Be it so (Sanskrit), City (Ancient Greek)
 *----------------------------------------------------------------------------
 */

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <limits>
#include "audio/WaveCodec.h"
#include "AstAudio.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// Class AudioSamples
    /////////////////////////////////////////////////

    AudioSamples::AudioSamples(unsigned int _sampleRate, unsigned int _numChannels)
        : sampleRate(_sampleRate)
        , numChannels(_numChannels)
    {
        // Intentionally left empty.
    }

    size_t AudioSamples::NumOfSamples() const
    {
        return samples.size();
    }

    size_t AudioSamples::NumOfFrames() const
    {
        // In case we have no samples at all.
        if (NumOfChannels() == 0) {
            return 0;
        }

        return samples.size() / NumOfChannels();
    }

    unsigned int AudioSamples::NumOfChannels() const
    {
        return numChannels;
    }

    unsigned int AudioSamples::GetSampleRate() const
    {
        return sampleRate;
    }

    std::vector<float> & AudioSamples::GetSamples()
    {
        return samples;
    }

    const std::vector<float> & AudioSamples::GetSamples() const
    {
        return samples;
    }

    /////////////////////////////////////////////////
    /////// Function LoadAudio
    /////////////////////////////////////////////////

    bool FindChunk(std::istream& is, const char *fourcc)
    {

        SkipChunk skip;

        skip.ReadFourCC(is);
        while (!skip.IsFourCC(fourcc) && is.good()) {
            skip.ReadWithoutFourCC(is);
            skip.ReadFourCC(is);
        }
        return is.good();
    }

    // Note: this implementation is neither elegant nor particularly fast and 
    // needs to be revised.
    std::unique_ptr<AudioSamples> LoadAudio(const std::string & filename)
    {
        // Open input stream.
        std::ifstream ifs(filename, std::ios::in | std::ios::binary);

        if (!ifs.good()) {
            throw std::runtime_error("Unable to open file for reading '" + filename + "'");
        }

        // Create utility objects used to read RIFF chunks.
        WaveChunk waveChunk;
        FormatChunk formatChunk;
        DataChunk dataChunk;

        // Read main chunk.
        waveChunk.Read(ifs);

        // Read audio format chunk.
        if (!FindChunk(ifs, formatChunk.GetFourCC())) {
            ifs.close();
            throw std::runtime_error("Unable to load WAV file '" + filename 
                + "'. The FORMAT chunk could not be found.");
        }
        formatChunk.ReadWithoutFourCC(ifs);

        // Read data chunk (without actual audio data).
        if (!FindChunk(ifs, dataChunk.GetFourCC())) {
            ifs.close();
            throw std::runtime_error("Unable to load WAV file '" + filename 
                + "'. The DATA chunk could not be found.");
        }
        dataChunk.ReadWithoutFourCC(ifs);


        // Read audio data.
        auto result = std::make_unique<AudioSamples>(formatChunk.GetSampleRate(), formatChunk.GetNumberOfChannels());

        if (formatChunk.GetAudioFormat() == FormatChunk::AudioFormat::IEEE_FLOAT) {
            // No conversion required, read plain floating-point values.

            assert(dataChunk.GetDataSize() % sizeof(float) == 0);
            size_t numSamples = dataChunk.GetDataSize() / sizeof(float);
            result->GetSamples().resize(numSamples);
            ifs.read(reinterpret_cast<char*>(result->GetSamples().data()), dataChunk.GetDataSize());
        } else if (formatChunk.GetAudioFormat() == FormatChunk::AudioFormat::PCM) {
            // We need to convert 8-bit or 16-bit PCM to IEEE float.
            
            if (formatChunk.GetBitsPerSample() == 16) {
                // Convert 16-bit samples to float.
        
                assert(dataChunk.GetDataSize() % sizeof(int16_t) == 0);
                size_t numSamples = dataChunk.GetDataSize() / sizeof(int16_t);
                std::unique_ptr<int16_t[]> temp = std::make_unique<int16_t[]>(numSamples);
                ifs.read(reinterpret_cast<char*>(temp.get()), dataChunk.GetDataSize());

                result->GetSamples().resize(numSamples);
                int maxValue = -std::numeric_limits<int16_t>::min();

                const float kToFloat = -1.0f / std::numeric_limits<int16_t>::min();
                for (size_t i = 0; i < numSamples; ++i) {
                    result->GetSamples()[i] = temp[i] * kToFloat;
                    assert(result->GetSamples()[i] <= 1.0f || result->GetSamples()[i] >= -1.0f);
                }
            } else if(formatChunk.GetBitsPerSample() == 24) {
                // Convert 24-bit samples to float.
                assert(dataChunk.GetDataSize() % 3 == 0);

                size_t numSamples = dataChunk.GetDataSize() / 3;
                std::unique_ptr<uint8_t[]> temp = std::make_unique<uint8_t[]>(dataChunk.GetDataSize());
                ifs.read(reinterpret_cast<char*>(temp.get()), dataChunk.GetDataSize());

                result->GetSamples().resize(numSamples);
                const float kToFloat = 1.0f / 8388608;

                for (size_t i = 0; i < numSamples; ++i) {
                    result->GetSamples()[i] = (temp[i * 3] << 8 | temp[i * 3 + 1] << 16 | temp[i * 3 + 2] << 24) / 2147483648.0f;
                    assert(result->GetSamples()[i] <= 1.0f || result->GetSamples()[i] >= -1.0f);
                }

            } else if (formatChunk.GetBitsPerSample() == 8) {
                // Convert 8-bit samples to float.

                size_t numSamples = dataChunk.GetDataSize();
                std::unique_ptr<uint8_t[]> temp = std::make_unique<uint8_t[]>(numSamples);
                ifs.read(reinterpret_cast<char*>(temp.get()), dataChunk.GetDataSize());

                result->GetSamples().resize(numSamples);

                const float kToFloat = 2.0f / std::numeric_limits<uint8_t>::max();
                for (size_t i = 0; i < numSamples; ++i) {
                    result->GetSamples()[i] = -1.0f + result->GetSamples()[i] * kToFloat;
                    assert(result->GetSamples()[i] <= 1.0f || result->GetSamples()[i] >= -1.0f);
                }
            } else {
                ifs.close();
                throw std::runtime_error("Unsupported audio file format, neither 32-bit IEEE float audio nor 16/8 bit PCM (format code = " 
                    + std::to_string(formatChunk.GetAudioFormat()) + ")");
            }

        } else {
            ifs.close();
            throw std::runtime_error("Unsupported audio file format, neither 32-bit IEEE float audio nor 16/8 bit PCM (format code = " 
                + std::to_string(formatChunk.GetAudioFormat()) + ")");
        }

        // Close stream.
        ifs.close();

        return result;
    }

    void StoreAudio(const AudioSamples & audio, const std::string & filename)
    {
        // Create file output stream.
        std::ofstream ofs(filename, std::ios::out | std::ios::binary);
        if (!ofs) {
            throw std::runtime_error("Unable to store audio file. The file '" + filename + "' could not be opnend for writing.");
        }

        if (audio.NumOfChannels() > std::numeric_limits<uint16_t>::max()) {
            throw std::logic_error("The number of channels is invalid for WAV file format. Maximum number of channels is "
                 + std::to_string(std::numeric_limits<uint16_t>::max()) + "got " + std::to_string(audio.NumOfChannels()) + ".");
        }

        FormatChunk formatChunk;
        formatChunk.SetAudioFormat(FormatChunk::AudioFormat::IEEE_FLOAT);
        formatChunk.SetBitsPerSamples(32);
        formatChunk.SetNumberOfChannels(static_cast<unsigned int>(audio.NumOfChannels()));
        formatChunk.SetSampleRate(static_cast<unsigned int>(audio.GetSampleRate()));
        formatChunk.Update();

        DataChunk dataChunk(static_cast<unsigned int>(audio.NumOfSamples() * sizeof(float)));

        WaveChunk waveChunk(formatChunk.GetSize() + dataChunk.GetSize());
        waveChunk.Write(ofs);
        formatChunk.Write(ofs);
        dataChunk.Write(ofs);

        ofs.write(reinterpret_cast<const char*>(audio.GetSamples().data()), dataChunk.GetDataSize());

        // Close stream.
        ofs.close();
    }

} // end of namespace