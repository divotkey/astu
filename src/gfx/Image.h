/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <memory>

#include "gfx/Color.h"

namespace astu {

    class Image {
    public:

        Image(int w, int h);
        ~Image();

        int GetWidth() const {
            return width;
        }

        int GetHeight() const {
            return height;
        }

        const Color & GetPixel(int x, int y) const;
        void SetPixel(int x, int y, const Color & c);

    private:
        int width;
        int height;

        std::unique_ptr<Color[]> data;

        void ValidateCoordinates(int x, int y) const;
    };

}