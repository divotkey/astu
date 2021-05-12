/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <string>

// Local includes.
#include "Service.h"

namespace astu {

    class ResourceService : public Service {
    public:


        resMngr.GetResource<Sprite>("paddle");

        void AddSpriteResource(const std::string& path);


    private:

    };

} // end of namespace
