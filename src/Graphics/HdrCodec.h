/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Libraries includes
#include <memory>
#include <iostream>
#include <array>
#include <vector>

namespace astu {

// Forward declaration.
class Image;

/**
 * Decodes HDR files to images.
 */
    class HdrDecoder {
    public:

        HdrDecoder();

        /**
         * Decodes an HDR file from an input stream.
         *
         * This method will not close the given input stream.
         *
         * @param is    the input stream
         * @return the newly created image containing the BMP data
         * @throws std::runtime_error in case of i/o problem or if the
         *      input stream does not contain a valid BMP file
         */
        std::unique_ptr<Image> Decode(std::istream &is) const;

        /**
         * Convenient method to read an image directly from a file.
         *
         * @param filename  the filename including the file path
         * @return the newly created image containing the BMP data
         * @throws std::runtime_error in case of i/o problem or if the
         *      input file does not contain a valid BMP file
         */
        std::unique_ptr<Image> Decode(const char *filename) const;

    private:
        static const std::array<unsigned char, 11> MagicNumber;
        mutable std::vector<unsigned char> buffer;

        void ReadText(const std::string& source, size_t &inOutPos, const std::string& text) const;
        int ReadInt(const std::string& source, size_t &inOutPos) const;
        void RgbeToFloat(unsigned char* rgbe, float &outRed, float &outGreen, float &outBlue) const;

    };

} // end of namespace