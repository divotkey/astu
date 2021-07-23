/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard includes
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

    /**
     * Returns the copyright holder of AST-Utilities.
     * 
     * @return the copyright holder
     */
    const std::string & GetCopyrightHolder();

    /**
     * Returns the copyright year of AST-Utilities.
     * 
     * @return the copyright year
     */
    const std::string & GetCopyrightYear();

    /**
     * Returns copyright information as string.
     * 
     * @return copyright information
     */
    std::string GetCopyrightInfo();

}