/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/CompoundPattern.h"

// C++ Standard Library includes
#include <algorithm>

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

    void CompoundPattern::RemovePattern(std::shared_ptr<Pattern> child)
    {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
        OnPatternRemoved(*child);
    }

} // end of namespace