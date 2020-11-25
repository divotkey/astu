/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>
#include <iostream>
#include <vector>

namespace astu {

    // Forward declaration.
    class Image;

    /**
     * Converts images into BMP files.
     */
    class BmpEncoder {
    public:

        /**
         * Constructor.
         */
        BmpEncoder();

        /**
         * Encodes the specified image to the given output stream.
         * This method will not close the given output stream.
         *
         * @param image the image to be encoded
         * @param os    the output stream
         */
        void Encode(const Image & image, std::ostream& os) const;

        /**
         * Convenient method to write an image directly into a file.
         *
         * @param image     the image to be encoded
         * @param filename  the filename including the file path
         * @throws std::runtime_error in case of an I/O error
         */
        void Encode(const Image & image, const char * filename) const;

        /**
         * Returns whether images will get flipped vertically.
         * This flags defines whether the image will be flipped
         * along the x-axis.
         */
        bool IsFlipVertically() const;

        /**
         * Defines whether the image should be flipped vertically.
         * 
         * This flags defines whether the image will be flipped
         * along the x-axis.
         * 
         * The default value of thie flag is set to `true`, which
         * is the common configuration for BMP files.
         * 
         * @param flip  set to `true` if images should be flipped
         */            
        void SetFlipVertically(bool flip);

    private:
        /** Whether the image shouled be flipped vertically. */
        bool flipVertically;
    };

    /**
     * Decodes BMP files to images.
     */
    class BmpDecoder {
    public:

        /**
         * Constructor.
         */
        BmpDecoder();

        /**
         * Decodes an BMP file from an input stream.
         * This method will not close the given input stream.
         *
         * @param is    the input stream
         * @return the newly created image containing the BMP data
         * @throws std::runtime_error in case of i/o problem or if the
         *      input stream does not contain a valid BMP file
         */
        std::unique_ptr<Image> Decode(std::istream& is) const;

        /**
         * Convenient method to read an image directly from a file.
         *
         * @param filename  the filename including the file path
         * @return the newly created image containing the BMP data
         * @throws std::runtime_error in case of i/o problem or if the
         *      input file does not contain a valid BMP file
         */
        std::unique_ptr<Image> Decode(const char * filename) const;

    private:
        /** Used to buffer input stream. */
        mutable std::vector<unsigned char> buffer;
    };

}
