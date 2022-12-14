/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/CompoundPattern.h"

// C++ Standard Library includes
#include <string>

namespace astu {

    void CompoundPattern::AddPattern(std::shared_ptr<Pattern> child)
    {
        children.push_back(child);
        OnPatternAdded(*child);        
    }

    void CompoundPattern::Clear()
    {
        children.clear();
        OnClear();
    }

} // end of namespace