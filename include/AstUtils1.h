/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

namespace astu1 {

    /**
     * Retrieves the message text for the last-error.
     * 
     * @return the last-error message text
     */ 
    const char* GetLastError();

    /**
     * Loads a BMP image from a file.
     * 
     * @param filename  the filename including the path
     * @return the handle to the image, or 0 if the operation failed
     */
    int LoadImage(const char *filename);

    /**
     * Stores an image to a file.
     * 
     * @param filename  the filename including the path
     * @return returns 0 on success or a negative error code on failure
     */
    int StoreImage(int hImg, const char *filename);

    /**
     * Removes an image from memory and invalidates its handle.
     * 
     * @param hImg  the handle of the image
     * @return returns 0 on success or a negative error code on failure
     */
    int DestroyImage(int hImg);

    /**
     * Returns the width of an image.
     * 
     * @param hImg  the handle of the image
     * @return the width of the image or a negative error code on failure
     */ 
    int GetImageWidth(int hImg);

    /**
     * Returns the height of an image.
     * 
     * @param hImg  the handle of the image
     * @return the width of the image or a negative error code on failure
     */ 
    int GetImageHeight(int hImg);

    /**
     * Exports the image to 32-bit floating-point RGB values.
     * 
     * @param hImg  the handle of the image
     * @return start address of the newly allocated memory block or nullptr on failure
     */
    float* ExportRgbFloats(int hImg);

    /**
     * Imports 32-bit floating-point RGB values to an image.
     * 
     * @param hImg  the handle of the image
     * @param ptr   start address RGB float data
     * @return returns 0 on success or a negative error code on failure
     */
    int ImportRgbFloats(int hImg, float* ptr);

    /**
     * Frees the memory allocated by 32-bit floating poitn RGB values.
     * 
     * @param ptr   start address the memory block
     * @return returns 0 on success or a negative error code on failure
     */
    int DestroyRgbFloats(float* ptr);

}