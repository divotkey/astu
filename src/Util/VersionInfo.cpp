/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#include "AstUtilsConfig.h"
#include "Util/VersionInfo.h"

namespace astu {

    const std::string kCopyrightHolder  = "AST Utilities Development Team";
    const std::string kCopyrightYear    = "2020 - 2022";

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

    const std::string & GetCopyrightHolder()
    {
        return kCopyrightHolder;
    }

    const std::string & GetCopyrightYear()
    {
        return kCopyrightYear;
    }

    std::string GetCopyrightInfo()
    {
        return "Copyright " + GetCopyrightYear() + " " + GetCopyrightHolder() + ". All rights reserved.";
    }
}