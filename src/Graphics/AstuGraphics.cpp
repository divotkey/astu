/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes    
#include "AstuGraphics.h"
#include "Graphics/BmpCodec.h"

namespace astu {

    /** Used to read BMP files. */
    static astu::BmpDecoder bmpDecoder;

    /** Used to write BMP files. */
    static astu::BmpEncoder bmpEncoder;

    void StoreImage(const Image & image, const std::string & filename)
    {
        bmpEncoder.Encode(image, filename.c_str());
    }

    std::unique_ptr<Image> LoadImage(const std::string & filename)
    {
        return bmpDecoder.Decode(filename.c_str());  
    }

} // end of namespace
