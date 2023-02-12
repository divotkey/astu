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

    void BlackboardServiceTs::OnStartup()
    {
        assert(stringMap.empty());
    }

    void BlackboardServiceTs::OnShutdown()
    {
        ClearStrings();
        ClearImages();
    }

    bool BlackboardServiceTs::HasString(const string &key) const
    {
        std::lock_guard<std::mutex> lock(stringMapMutex);
        return stringMap.find(key) != stringMap.end();
    }

    void BlackboardServiceTs::SetString(const string &key, const string &value)
    {
        std::lock_guard<std::mutex> lock(stringMapMutex);
        stringMap[key] = value;
    }

    const std::string BlackboardServiceTs::GetString(const string &key) const
    {
        std::lock_guard<std::mutex> lock(stringMapMutex);
        auto it = stringMap.find(key);
        if (it == stringMap.end()) {
            throw std::logic_error("Unable to retrieve string value from blackboard, unknown key '" + key + "'");
        }

        return it->second;
    }

    void BlackboardServiceTs::ClearStrings()
    {
        std::lock_guard<std::mutex> lock(stringMapMutex);
        stringMap.clear();
    }

    bool BlackboardServiceTs::HasImage(const string &key) const
    {
        std::lock_guard<std::mutex> lock(imageMapMutex);
        return imageMap.find(key) != imageMap.end();
    }

    void BlackboardServiceTs::SetImage(const string &key, std::shared_ptr<Image> value)
    {
        std::lock_guard<std::mutex> lock(imageMapMutex);
        imageMap[key] = value;
    }

    std::shared_ptr<Image> BlackboardServiceTs::GetImage(const string &key) const
    {
        std::lock_guard<std::mutex> lock(imageMapMutex);
        auto it = imageMap.find(key);
        if (it == imageMap.end()) {
            throw std::logic_error("Unable to retrieve image from blackboard, unknown key '" + key + "'");
        }

        return it->second;
    }

    void BlackboardServiceTs::ClearImages()
    {
        std::lock_guard<std::mutex> lock(imageMapMutex);
        imageMap.clear();
    }

} // end of namespace