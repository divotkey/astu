/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Service/BlackboardServiceTs.h"

// C++ Standard Library includes
#include <cassert>

using namespace std;

namespace astu {

    BlackboardServiceTs::BlackboardServiceTs() : Service("Blackboard Service (TS)")
    {
        // Intentionally left empty.
    }

    bool BlackboardServiceTs::HasStringTs(const string &key) const
    {
        std::lock_guard<std::mutex> lock(stringMapMutex);
        return stringMap.find(key) != stringMap.end();
    }

    void BlackboardServiceTs::SetStringTs(const string &key, const string &value)
    {
        std::lock_guard<std::mutex> lock(stringMapMutex);
        stringMap[key] = value;
    }

    const std::string BlackboardServiceTs::GetStringTs(const string &key) const
    {
        std::lock_guard<std::mutex> lock(stringMapMutex);
        auto it = stringMap.find(key);
        if (it == stringMap.end()) {
            throw std::logic_error("Unable to retrieve string value from blackboard, unknown key '" + key + "'");
        }

        return it->second;
    }

    void BlackboardServiceTs::OnStartup()
    {
        assert(stringMap.empty());
    }

    void BlackboardServiceTs::OnShutdown()
    {
        stringMap.clear();
    }

} // end of namespace