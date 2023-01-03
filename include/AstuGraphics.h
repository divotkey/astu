/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

/**
 * @file
 * @brief This file defines public functions, templates and classes, offered by the graphics module of AST-Utilities.
 */

#pragma once

// C++ Standard Library includes
#include <string>
#include <memory>

// ASTU Library includes
#include "Graphics/Color.h"
#include "Graphics/ColorHsv.h"
#include "Graphics/WebColors.h"
#include "Graphics/RalColors.h"
#include "Graphics/Palette.h"
#include "Graphics/Image.h"
#include "Graphics/ImageRenderer.h"
#include "Graphics/Texture.h"
#include "Graphics/Font.h"

namespace astu {

    /**
     * @defgroup gfx_group Graphics
     * @brief This module contains classes and functions dedicated to graphics.
     * @{
     */

    /**
     * Stores an image as BMP file.
     * 
     * @param image     the the image to be stored
     * @param filename  the filename including the path
     * @throws std::runtime_error in case of an I/O error
     */
    void StoreImage(const Image &image, const std::string &filename);

    /**
     * Loads an BMP image from file.
     * 
     * @param filename  the filename including the path
     * @return the loaded image
     * @throws std::runtime_error in case of an I/O error
     */
    std::unique_ptr<Image> LoadImage(const std::string & filename);

/**@}*/

} // namespace astu