/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 * 
 *----------------------------------------------------------------------------
 * Meanings of ASTU: Slightly melted butter (Hinduism / Religious Law), 
 * Let it be / Be it so (Sanskrit), City (Ancient Greek)
 *----------------------------------------------------------------------------
 */

#include <stdexcept>
#include <iostream>
#include <fstream>
#include "audio/WaveCodec.h"
#include "AstAudio.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// Class AudioSamples
    /////////////////////////////////////////////////

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

    std::vector<float> AudioSamples::GetSamples()
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

        auto result = std::unique_ptr<AudioSamples>();

        // Read audio data.

        return result;
    }

} // end of namespace