/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "AstuApi.h"

// C++ Standard includes
#include <string>

namespace astu {

    /**
     * Returns version information.
     * 
     * @return string containing version information of ASTU
     */
    ASTU_API std::string GetVersionInfo();

    /**
     * Returns major version number of AST-Utilities.
     * 
     * @return major version number
     */
    ASTU_API int GetMajorVersion();

    /**
     * Returns minor version number of AST-Utilities.
     * 
     * @return minor version number
     */
    ASTU_API int GetMinorVersion();

    /**
     * Returns patch version number of AST-Utilities.
     * 
     * @return patch version number
     */
    ASTU_API int GetPatchVersion();

    /**
     * Returns the copyright holder of AST-Utilities.
     * 
     * @return the copyright holder
     */
    ASTU_API const std::string & GetCopyrightHolder();

    /**
     * Returns the copyright year of AST-Utilities.
     * 
     * @return the copyright year
     */
    ASTU_API const std::string & GetCopyrightYear();

    /**
     * Returns copyright information as string.
     * 
     * @return copyright information
     */
    ASTU_API std::string GetCopyrightInfo();
}