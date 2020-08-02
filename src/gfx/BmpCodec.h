/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <iostream>

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
         * @param filename  the file name including the file path
         */
        void Encode(const Image & image, const char * filename) const;

        /**
         * Returns whether images will get flipped vertically.
         * This flags defines whether the image will be flipped
         * along the x-axis when encoded to a file.
         */
        bool IsFlipVertically() const;

        /**
         * Defines whether the image should be flipped vertically.
         * 
         * This flags defines whether the image will be flipped
         * along the x-axis when encoded to a file.
         * 
         * The default value of thie flag is set to `true`, which
         * is the common configuration for BMP files.
         * 
         * @param flip  set to `true` if images should be flipped
         * @return reference to this encoder for method chaining
         */            
        BmpEncoder & SetFlipVertically(bool flip);

    private:
        /** Whether the image shouled be flipped vertically. */
        bool flipVertically;
    };

}
