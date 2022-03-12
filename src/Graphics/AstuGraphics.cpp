/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes    
#include "AstuGraphics.h"
#include "Graphics/BmpCodec.h"
#include "Graphics/HdrCodec.h"
#include "Util/StringUtils.h"

namespace astu {

    /** Used to read BMP files. */
    static astu::BmpDecoder bmpDecoder;

    /** Used to write BMP files. */
    static astu::BmpEncoder bmpEncoder;

    /** Used to read HDR files. */
    static astu::HdrDecoder hdrDecoder;

    void StoreImage(const Image & image, const std::string & filename)
    {
        bmpEncoder.Encode(image, filename.c_str());
    }

    std::unique_ptr<Image> LoadImage(const std::string & filename)
    {
        auto extension = StringUtils::ExtractFileExtension(filename, false);
        StringUtils::toUpperCase(extension);

        if (extension == "BMP") {
            return bmpDecoder.Decode(filename.c_str());
        } else if (extension == "HDR") {
            return hdrDecoder.Decode(filename.c_str());
        } else {
            throw std::runtime_error("Unsupported image format");
        }
    }

} // end of namespace
