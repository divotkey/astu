/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Library includes
#include <vector>

// Local includes
#include "Graphics/Color.h"

namespace astu {

    /**
     * This class represents an image where each individual pixel is
     * represented as instances of the class Color. The Color class
     * represents a pixel as RGBA (red, green, blue and alpha channel)
     * floating-point components.
     * 
     * Representing an image this way offers a convenient and straightforward
     * way and maintains high color precision. However, memory consumption and
     * performance might suffer. This class is preferably used for image 
     * synthesis and analysis.
     * 
     * @ingroup gfx_group
     */
    class Image {
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
         * Virtual destructor.
         */
        virtual ~Image() {}

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
         * Returns the aspect ratio of this image.
         * 
         * @return the aspect ratio
         */
        double GetAspectRatio() const {
            return static_cast<double>(width) / height;
        }

        /**
         * Returns the color of a pixel.
         * 
         * @param x the x-coordinate of the pixel
         * @param y the y-coordinate of the pixel
         * @return the color at the specified location
         * @throws std::out_of_range in case the coordinates are invalid
         */
        const Color4d & GetPixel(int x, int y) const;

        /**
         * Sets the color of a pixel.
         * 
         * @param x the x-coordinate of the pixel
         * @param y the y-coordinate of the pixel
         * @param c the new color
         * @throws std::out_of_range in case the coordinates are invalid
         */
        void SetPixel(int x, int y, const Color4d & c);

        /**
         * Returns the color of a pixel.
         * 
         * @param idx   the linear index of the pixel
         * @return the color of the pixel at the specified location
         * @throws std::out_of_range in case the index is invalid
         */
        const Color4d & GetPixel(size_t idx) const;

        /**
         * Sets the color of a pixel.
         * 
         * @param idx   the linear index of the pixel
         * @param c     the new color of the pixel at the specified location
         * @throws std::out_of_range in case the index is invalid
         */
        void SetPixel(size_t idx, const Color4d & c);

        /**
         * Returns the number of pixels of this image.
         * 
         * The number of pixels equals width times height of this image.
         * 
         * @return the number of pixels
         */
        size_t NumberOfPixels() const;

        /**
         * Normalizes this image such that the maximum value of any color channel is 1.
         *
         * @return reference to this image for method chaining
         */
        Image& Normalize();

        /**
         * Searches for the maximum channel value in this image.
         *
         * @return the maximum channel value, either red, green, blue or alpha of the brightest pixel
         */
        double GetMaxValue() const;

        /**
         * This method provides raw access to the pixel colors, use with care.
         * 
         * @return pointer to the linear array of pixel colors
         */
        Color4d* GetPixels();

        /**
         * This method provides raw access to the pixel colors, use with care.
         * 
         * This version returns a const pointer to the array and does not allow
         * the modify the color values.
         * 
         * @return pointer to the linear array of pixel colors
         */
        const Color4d* GetPixels() const;

    private:
        /** The width of the image in pixel. */
        int width;

        /** The height of the image in pixel. */
        int height;

        /** The image data, stored as linear vector of Color instances. */
        std::vector<Color4d> data;

        /**
         * Validates pixel coordinates.
         * This methods validates whether x, y coordinates lie within the
         * range of the dimension of this image. If the validation fails,
         * an exception is thrown.
         * 
         * @param x the x-coordinate to validate
         * @param y the y-coordinate to validate
         * @throws std::out_of_range in case the coordinates are invalid
         */
        void ValidateCoordinates(int x, int y) const;

        /**
         * Validates the pixel index.
         * 
         * @param idx   the index to validate
         * @throws std::out_of_range in case the index is invalid
         */
        void ValidateIndex(size_t idx) const;
    };

}