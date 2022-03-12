/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/Image.h"
#include "Graphics/WebColors.h"

// C++ Standard Library includes
#include <stdexcept>
#include <string>
#include <limits>
#include <algorithm>

namespace astu {

    // Note: the `ValidateIndex(...)` and `ValidateCoordinates(...)`
    // are used to generate more detailed exception messages.
    // An alternative would be to use the `at()` method of 
    // std::vector, which would generate an out_of_range exception
    // as well, but with less information.

    Image::Image(int w, int h)
        : width(w)
        , height(h)
    {
        if (w <= 0) {
            throw std::domain_error("Image width must be greater zero, got " 
                + std::to_string(w));
        }

        if (h <= 0) {
            throw std::domain_error("Image height must be greater zero, got " 
                + std::to_string(h));
        }

        // Resize vector of colors to hold the pixels
        // and initializes the colors using black.
        data.resize(width * height, WebColors::Black);
    }

    const Color4d & Image::GetPixel(int x, int y) const
    {
        ValidateCoordinates(x, y);
        return data[y * width + x];
    }

    void Image::SetPixel(int x, int y, const Color4d & c)
    {
        ValidateCoordinates(x, y);
        data[y * width + x] = c;
    }

    const Color4d & Image::GetPixel(size_t idx) const
    {
        ValidateIndex(idx);
        return data[idx];
    }

    void Image::SetPixel(size_t idx, const Color4d & c)
    {
        ValidateIndex(idx);
        data[idx] = c;
    }

    size_t Image::NumberOfPixels() const
    {
        return data.size();
    }

    Color4d* Image::GetPixels()
    {
        return data.data();
    }

    const Color4d* Image::GetPixels() const
    {
        return data.data();
    }

    void Image::ValidateCoordinates(int x, int y) const
    {
        if (x >= width) {
            throw std::out_of_range("The x-coordinate exceeds image width, got " 
                + std::to_string(x));
        }

        if (x < 0) {
            throw std::out_of_range("The x-coordinate must be greater zero, got " 
                + std::to_string(x));
        }

        if (y >= height) {
            throw std::out_of_range("The y-coordinate exceeds image width, got " 
                + std::to_string(y));
        }

        if (y < 0) {
            throw std::out_of_range("The y-coordinate must be greater zero, got " 
                + std::to_string(y));
        }
    }

    void Image::ValidateIndex(size_t idx) const
    {
        if (idx >= data.size()) {
            throw std::out_of_range("The pixel index exceeds the number of pixels, got "
                + std::to_string(idx));
        }
    }

    Image& Image::Normalize() {
        double s = std::numeric_limits<double>::lowest();
        for (const auto &c : data) {
            s = std::max(s, c.GetMax());
        }

        for (auto &c : data) {
            c /= s;
        }

        return *this;
    }

    double Image::GetMaxValue() const {
        double s = std::numeric_limits<double>::lowest();
        for (const auto &c : data) {
            s = std::max(s, c.GetMax());
        }

        return s;
    }

}