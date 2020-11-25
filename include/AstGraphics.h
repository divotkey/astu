/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 * 
 *----------------------------------------------------------------------------
 * Meanings of ASTU: Slightly melted butter (Hinduism / Religious Law), 
 * Let it be / Be it so (Sanskrit), City (Ancient Greek)
 *----------------------------------------------------------------------------
 */

/**
 * @file
 * @brief This file defines public functions offered by the graphics module of AST utilities.
 */

#pragma once

#include <string>
#include <memory>
#include "Color.h"
#include "Palette.h"
#include "Image.h"

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