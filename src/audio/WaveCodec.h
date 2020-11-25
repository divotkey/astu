/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <iostream>
#include <memory>

namespace astu {

    class RiffChunk {
    public:

        /**
         * Constructor.
         * 
         * @param code  the four-character code of this chunk
         * @param size  the size of the data (chunk size - 8 bytes)
         */
        RiffChunk(const char* code, unsigned int size);

        /**
         * Virtual destructor.
         */ 
        virtual ~RiffChunk() {}

        /**
         * Returns the four-character code of this chunk.
         * 
         * @return the four-character code
         */
        const char* GetFourCC() const;

        /**
         * Tests whether the given code matches the fourCC of this chunk.
         * 
         * @return `true` if the code is the fourCC of this chunk
         */
        bool IsFourCC(const char *code) const;

        /**
         * Returns the number of bytes occupied by the data of this chunk.
         * 
         * @return the data size in bytes
         */
        unsigned int GetDataSize() const;

        /**
         * Returns the size of this chunk including header information.
         * 
         * @return the size in bytes
         */
        unsigned int GetSize() const;

        /**
         * Writes this chunk to an output stream.
         * 
         * @param os    the output stream
         */
        void Write(std::ostream& os) const;

        /**
         * Reads this chunk without its FourCC from an input stream.
         * 
         * @param is    the input stream
         * @throws std::runtime_error in case of an I/O error or if chunk data are invalid
         */
        void ReadWithoutFourCC(std::istream& is);

        /**
         * Reads the FourCC from an input stream.
         * 
         * The behavior of this method depends on the concrete chunk-subclass.
         * Most chunks read the four-character code from the input stream and
         * throw an exception in case the read code does not match the code of the chunk.
         * 
         * @param is    the input stream
         * @throws std::runtime_error in case of an I/O error or FourCC is invalid
         */
        virtual void ReadFourCC(std::istream& is);

        /**
         * Reads this chunk.
         * 
         * @param is    the input stream
         * @throws std::runtime_error in case of an I/O error or if chunk data are invalid
         */
        void Read(std::istream& is);

    protected:
        /** The four-character code (FourCC). */
        char fourcc[4];

        void Write2ByteUInt(std::ostream& os, uint16_t value) const;
        void Write4ByteUInt(std::ostream& os, uint32_t value) const;
        unsigned int Read2ByteUInt(std::istream& is) const;
        unsigned int Read4ByteUInt(std::istream& is) const;
        void ValidateStream(std::istream& is) const;
        virtual void WriteBody(std::ostream& os) const = 0;
        virtual void ReadBody(std::istream& is) = 0;

    private:
        /** The size of data inside this chunk in bytes. */
        unsigned int dataSize;
    };

    class WaveChunk : public RiffChunk {
    public:

        /**
         * Constructor.
         * 
         * This chunk is a container chunk hosting all other chunks of the wave file,
         * its size is the size of the entire file - 8 bytes.
         * 
         * @param subChunkSize      the size of sub-chunks
         */
        WaveChunk(unsigned int subChunkSize = 0);

        /**
         * Virtual destructor.
         */ 
        virtual ~WaveChunk() {}

    protected:

        // Inherited via RiffChunk.
        virtual void WriteBody(std::ostream& os) const override;
        virtual void ReadBody(std::istream& is) override;

    private:
        /** Type, must be 'WAVE' */
        char type[4];
    };

    class FormatChunk : public RiffChunk {
    public:

        enum AudioFormat {
            PCM                 = 0x0001, 
            MS_ADPCM            = 0x0002, 
            IEEE_FLOAT          = 0x0003,
            ALAW                = 0x0006,
            MULAW               = 0x0007,
            MPEG_1_Layer_I_II   = 0x0050,
            MPEG_1_Layer_III    = 0x0055,
        };

        /**
         * Constructor.
         */
        FormatChunk();

        /**
         * Virtual destructor.
         */ 
        virtual ~FormatChunk() {}

        /**
         * Returns the number of channels.
         *  
         * @return the number of channels
         */
        unsigned int GetNumberOfChannels() const;

        /**
         * Returns the number of channels.
         * 
         * @return the number of channels
         * @throws std::domain_error in case the specified number of channels is invalid
         */
        void SetNumberOfChannels(unsigned int ch);

        AudioFormat GetAudioFormat() const;

        void SetAudioFormat(AudioFormat format);

        unsigned int GetSampleRate() const;
        void SetSampleRate(unsigned int rate);

        unsigned int GetBitsPerSample() const;
        void SetBitsPerSamples(unsigned int numBits);

        /**
         * Returns the size of one frame in bytes.
         * A frame consists audio data for all channels including padding.
         * 
         * @return the frame size in bytes
         */
        unsigned int GetBlockAlign() const;

        void SetBlockAlign(unsigned int numBytes);

        /**
         * Calculates the frame size based on current configuration.
         * The calculates value will be stored int he block align property.
         */
        void UpdateBlockAlign();

        unsigned GetByteRate() const;
        void SetByteRate(unsigned int bytesPerSecond);

        /**
         * Calculates the byte rate based on the current configuration.
         * This method requires to block align property (frame size) to be
         * set correctly.
         */
        void UpdateByteRate();

        /**
         * Calculates the frame size and byte rate based on the current configuration.
         */
        void Update();

    protected:

        // Inherited via RiffChunk.
        virtual void WriteBody(std::ostream& os) const override;
        virtual void ReadBody(std::istream& is) override;

    private:
        /** The expected data size of this chunk. */
        static const unsigned int kDataSize;

        /** Describes the audio format. */
        AudioFormat audioFormat;

        /** The number of channels the audio signal contains. */
        unsigned int numberOfChannels;

        /** Number of samples per second. */
        unsigned int sampleRate;

        /** The number of bits per sample. */
        unsigned int bitsPerSample;

        /** Bytes per second. */
        unsigned int byteRate;

        /** The size of one frame. */
        unsigned int blockAlign;
    };

    class DataChunk : public RiffChunk {
    public:

        /**
         * Constructor.
         * 
         * @param dataSize  the size of the chunk data in bytes
         */
        DataChunk(unsigned int dataSize = 0);

        /**
         * Virtual destructor.
         */ 
        virtual ~DataChunk() {}

    protected:

        // Inherited via RiffChunk.
        virtual void WriteBody(std::ostream& os) const override {}
        virtual void ReadBody(std::istream& is) override {}
    };

    /**
     * This dummy chunk can be used to skip unknown chunks.
     */
    class SkipChunk : public RiffChunk {
    public:

        /**
         * Constructor.
         * 
         * @param code  the four-character code of this chunk
         */
        SkipChunk(const char* code = nullptr);

        // Inherited via RiffChunk.
        virtual void ReadFourCC(std::istream& is) override;

    protected:
        // Inherited via RiffChunk.
        virtual void WriteBody(std::ostream& os) const override;
        virtual void ReadBody(std::istream& is) override;
    };


    // class WaveCodec {
    // public:

    // private:
    // }

    // struct WavFormatChunk {

    // }


    /**
     * Converts raw sample data to RIFF WAVE files.
     */
    class WaveEncoder {
    public:

        // void Encode(float *samples, size numSamples, std::ostream& os);

        // /**
        //  * Convenient method to write an image directly into a file.
        //  *
        //  * @param image     the image to be encoded
        //  * @param filename  the filename including the file path
        //  */
        // void Encode(const Image & image, const char * filename) const;


        // void SetNumChannels(int n);
        // int getNumChannels() const;

    private:

        int numChannels;


    };

}