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
    
#include "gfx/BmpCodec.h"
#include "AstGraphics.h"

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
