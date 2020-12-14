/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include "AstUtilsConfig.h"
#include "VersionInfo.h"

namespace astu {

    std::string GetVersionInfo()
    {
        return std::string("AST Utilities Version ") + ASTU_VERSION_STRING 
            + " (" + std::to_string((sizeof(void*) * 8)) + " bit address space)";
    }

    int GetMajorVersion() {
        return ASTU_VERSION_MAJOR;
    }

    int GetMinorVersion() {
        return ASTU_VERSION_MINOR;
    }

    int GetPatchVersion() {
        return ASTU_VERSION_PATCH;
    }

}