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

#include <string>
#include <vector>
#include <memory>

namespace astu {

    class AudioSamples {
    public:

        /**
         * Constructor.
         * 
         * @param 
         */
        AudioSamples(unsigned int sampleRate, unsigned int numChannels);

        /**
         * Returns the number of samples.
         * 
         * This method returns the total number of samples including all
         * audio channels.
         * 
         * @return the number of samples
         */
        size_t NumOfSamples() const;

        /**
         * Returns the number of frames.
         * 
         * A frame consist of sample data for all interleaved channels at a
         * specific position in time. If there is only one channel stored
         * in this buffer, the number of frames is equal to the number of 
         * samples. For two channel audio data, the number of frames is
         * half the number of samples etc.
         * 
         * @return the number of audio frames
         */
        size_t NumOfFrames() const;

        /**
         * Returns the number of audio channels.
         * 
         * @return the number of interleaved channels
         */
        unsigned int NumOfChannels() const;

        /**
         * Returns the sample rate of the audio data.
         * 
         * @return the sample rate in frames per second
         */
        unsigned int GetSampleRate() const;

        std::vector<float> & GetSamples();

        const std::vector<float> & GetSamples() const;

    private:
        /** The audio samples, containing interleaved audio channels. */
        std::vector<float> samples;

        /** The number of audio channels. */
        unsigned int numChannels;

        /** The sample rate in frames per second. */
        unsigned int sampleRate;
    };

    std::unique_ptr<AudioSamples> LoadAudio(const std::string & filename);
    void StoreAudio(const AudioSamples & audio, const std::string & filename);

};