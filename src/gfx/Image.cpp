/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include <string>
#include "Image.h"

namespace astu {

    Image::Image(int w, int h)
        : width(w)
        , height(h)
    {
        if (w <= 0) {
            throw std::domain_error("image width must be greater zero, got " 
                + std::to_string(w));
        }

        if (h <= 0) {
            throw std::domain_error("image height must be greater zero, got " 
                + std::to_string(h));
        }

        data = std::make_unique<Color[]>(width * height);
    }

    Image::~Image()
    {
        // Intentionally left empty.
    }

    const Color & Image::GetPixel(int x, int y) const
    {
        ValidateCoordinates(x, y);
        return data[y * width + x];
    }

    void Image::SetPixel(int x, int y, const Color & c)
    {
        ValidateCoordinates(x, y);
        data[y * width + x] = c;
    }

    void Image::ValidateCoordinates(int x, int y) const
    {
        if (x >= width) {
            throw std::domain_error("x coordinate exceeds image width, got " 
                + std::to_string(x));
        }

        if (x < 0) {
            throw std::domain_error("x coordinate must be greater zero, got " 
                + std::to_string(x));
        }

        if (y >= height) {
            throw std::domain_error("y coordinate exceeds image width, got " 
                + std::to_string(y));
        }

        if (y < 0) {
            throw std::domain_error("y coordinate must be greater zero, got " 
                + std::to_string(y));
        }
    }
}