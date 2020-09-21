/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>

#include "Color.h"

namespace astu {

    /**
     * This class represents an image where each individual pixel is
     * represented as instances of the class Color. The Color class
     * represents a pixel as RGBA (red, green, blue and alpha channel)
     * floating-point component.
     * 
     * Representing an image this way offers a convenient and straightforward
     * way and maintains high color precision. However, memory consumption and
     * performance might suffer. This class is preferably used for image 
     * synthesis and analysis.
     */
    class Image final {
    public:

        /**
         * Constructor.
         * 
         * @param w the width of the image in pixels
         * @param h the height of the image in pixels
         * @throws std::domain_error in case the width or height is
         */
        Image(int w, int h);

        /**
         * Returns the width of this image
         * 
         * @return the width of this image in pixel
         */
        int GetWidth() const {
            return width;
        }

        /**
         * Returns the height of this image
         * 
         * @return the height of this image in pixel
         */
        int GetHeight() const {
            return height;
        }

        /**
         * Returns the color of a pixel.
         * 
         * @param x the x-coordinate of the pixel
         * @param y the y-coordinate of the pixel
         * @return the color at the specified location
         * @throws std::domain_error in case the coordinates are invalid
         */
        const Color & GetPixel(int x, int y) const;

        /**
         * Sets the color of a pixel.
         * 
         * @param x the x-coordinate of the pixel
         * @param y the y-coordinate of the pixel
         * @param c the color
         * @throws std::domain_error in case the coordinates are invalid
         */
        void SetPixel(int x, int y, const Color & c);

    private:
        /** The width of the image in pixel. */
        int width;

        /** The height of the image in pixel. */
        int height;

        /** The image data, stored as linear array of colors. */
        std::unique_ptr<Color[]> data;

        /**
         * Validates pixel coordinates.
         * This methods validates whether x, y coordinates lie within the
         * range of the dimension of this image. If the validation fails,
         * an exception is thrown.
         * 
         * @param x the x-coordinate to validate
         * @param y the y-coordinate to validate
         * @throws std::domain_error in case the coordinates are invalid
         */
        void ValidateCoordinates(int x, int y) const;
    };

}