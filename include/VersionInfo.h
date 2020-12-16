/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

#include <string>

namespace astu {

    /**
     * Returns version information.
     * 
     * @return string containing version information of ASTU
     */
    std::string GetVersionInfo();

    /**
     * Returns major version number of AST-Utilities.
     * 
     * @return major version number
     */
    int GetMajorVersion();

    /**
     * Returns minor version number of AST-Utilities.
     * 
     * @return minor version number
     */
    int GetMinorVersion();

    /**
     * Returns patch version number of AST-Utilities.
     * 
     * @return patch version number
     */
    int GetPatchVersion();


}