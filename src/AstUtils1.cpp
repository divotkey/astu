/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <map>
#include <iostream>
#include "Image.h"
#include "gfx/BmpCodec.h"
#include "AstUtils1.h"

namespace astu1 {

    struct RgbFloatData {
        int width;
        int height;
    };


    /** The global storage for images, associated with handles. */
    static std::map<int, std::unique_ptr<astu::Image>> images;

    /** Used to read BMP files. */
    static astu::BmpDecoder bmpDecoder;

    /** Used to write BMP files. */
    static astu::BmpEncoder bmpEncoder;

    /** Incremented to create new unique image handles. */
    static int imageCounter = 0;

    /** Used to store additional information about extracted RGB float data. */
    static std::map<float*, RgbFloatData> rgbFloatDataMap;

    /** Constant indicating an invalid handle. */
    static const int INVALID_HANDLE = 0;

    /** Return code for functions to indicate success. */
    static const int ERR_SUCCESS = 0;

    /** Return code for functions to indicate failure. */
    static const int ERR_FAILED = -1;

    /** The contains the message text of the last-error. */
    std::string lastErrorText;



    /////////////////////////////////////////////////
    /////// Internal Utility Functions
    /////////////////////////////////////////////////

    bool HasImage(int hImg)
    {
        return images.find(hImg) != images.end();
    }

    void SetLastErrorX(const std::string & txt)
    {
        lastErrorText = txt;
    }

    bool hasRgbFloatData(float *ptr)
    {
        return rgbFloatDataMap.find(ptr) != rgbFloatDataMap.end();
    }

    float* AllocateRgbFloatData(int width, int height)
    {
        try{
            float* ptr = new float[width * height * 3];
            RgbFloatData fltData;
            fltData.width = width;
            fltData.height = height;

            assert(!hasRgbFloatData(ptr));
            rgbFloatDataMap[ptr] = fltData;
            return ptr;
        }
        catch(std::bad_alloc&) {
            return nullptr;
        }

    }

    bool FreeRgbFloatData(float * ptr)
    {
        auto it = rgbFloatDataMap.find(ptr);
        if (it == rgbFloatDataMap.end()) {
            return false;
        }

        delete [] ptr;
        rgbFloatDataMap.erase(it);
        return true;
    }

    /////////////////////////////////////////////////
    /////// Implementation of ASTU, API Level 1
    /////////////////////////////////////////////////

    const char* GetLastErrorX()
    {
        if (lastErrorText.empty()) {
            return "no error";
        } 

        return lastErrorText.c_str();
    }

    int LoadImage(const char *filename) 
    {
        assert(!HasImage(imageCounter + 1));

        try {
            images[++imageCounter] = bmpDecoder.Decode(filename);
            return imageCounter;
        } catch (std::runtime_error & e) {
            std::cerr << e.what() << std::endl;
            SetLastErrorX(e.what());
            return INVALID_HANDLE;
        }

    }

    int CreateImage(int width, int height)
    {
        images[++imageCounter] = std::make_unique<astu::Image>(width, height);
        return imageCounter;
    }

    int StoreImage(int hImg, const char *filename)
    {
        auto it = images.find(hImg);
        if (it == images.end())  {
            SetLastErrorX("invalid image handle");
            return ERR_FAILED;
        }

        try {
            bmpEncoder.Encode(*it->second, filename);
        } catch (std::runtime_error & e) {
            SetLastErrorX(e.what());
            return ERR_FAILED;
        }

        return ERR_SUCCESS;
    }

    int DestroyImage(int hImg)
    {
        auto it = images.find(hImg);
        if (it == images.end()) {
            SetLastErrorX("invalid image handle");
            return ERR_FAILED;
        }
        images.erase(it);
        return ERR_SUCCESS;
    } 

    int GetImageWidth(int hImg)
    {
        auto it = images.find(hImg);
        if (it == images.end()) {
            SetLastErrorX("invalid image handle");
            return ERR_FAILED;
        }

        return it->second->GetWidth();
    }

    int GetImageHeight(int hImg)
    {
        auto it = images.find(hImg);
        if (it == images.end()) {
            SetLastErrorX("invalid image handle");
            return ERR_FAILED;
        }

        return it->second->GetHeight();
    }

    astu::Color GetPixel(int hImg, int x, int y)
    {
        auto it = images.find(hImg);
        if (it == images.end()) {
            SetLastErrorX("invalid image handle");
            return ERR_FAILED;
        }

        return it->second->GetPixel(x, y);
    }


    float* ExportRgbFloats(int hImg)
    {
        auto it = images.find(hImg);
        if (it == images.end()) {
            SetLastErrorX("invalid image handle");
            return nullptr;
        }
        astu::Image & image = *it->second;

        auto result = AllocateRgbFloatData(image.GetWidth(), image.GetHeight());
        if (!result) {
            SetLastErrorX("unable to allocate memory for 32-bit RGB float data");
            return nullptr;
        }

        float *ptr = result;
        for (int j = 0; j < image.GetHeight(); ++j) {
            for (int i = 0; i < image.GetWidth(); ++i) {
                auto & c = image.GetPixel(i, j);
                *ptr++ = static_cast<float>(c.r);
                *ptr++ = static_cast<float>(c.g);
                *ptr++ = static_cast<float>(c.b);
            }
        }

        return result;
    }

    int ImportRgbFloats(int hImg, float* ptr)
    {
        auto it1 = rgbFloatDataMap.find(ptr);
        if (it1 == rgbFloatDataMap.end()) {
            SetLastErrorX("unknown 32-bit RGB float data");
            return ERR_FAILED;
        }

        auto it2 = images.find(hImg);
        if (it2 == images.end()) {
            SetLastErrorX("invalid image handle");
            return ERR_FAILED;
        }

        const auto & rgbFloat = it1->second;
        auto & image = *it2->second;

        if (rgbFloat.width != image.GetWidth() || rgbFloat.height != image.GetHeight()) {
            SetLastErrorX("invalid image size");
            return ERR_FAILED;
        } 

        for (int j = 0; j < image.GetHeight(); ++j) {
            for (int i = 0; i < image.GetWidth(); ++i) {
                astu::Color c;
                c.r = *ptr++;
                c.g = *ptr++;
                c.b = *ptr++;
                image.SetPixel(i, j, c);
            }
        }        

        return ERR_SUCCESS;
    }

    int DestroyRgbFloats(float* ptr)
    {
        if (!FreeRgbFloatData(ptr)) {
            SetLastErrorX("unknown 32-bit RGB float data");
            return ERR_FAILED;
        }

        return ERR_SUCCESS;
    }
}