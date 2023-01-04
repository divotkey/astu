/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Velox/Service/VeloxExtensionService.h"
#include "Velox/Service/VeloxConstants.h"

// C++ Standard Library includes
#include <algorithm>

using namespace velox;
using namespace std;

namespace astu {

    VeloxExtensionService::VeloxExtensionService() : Service("Velox Extension Service")
    {
        // Intentionally left empty.
    }

    void VeloxExtensionService::AddExtension(std::shared_ptr<VeloxExtension> extension)
    {
        if (HasExtension(extension)) {
            throw std::logic_error("Velox extension '" + extension->GetName() + "' already added");
        }

        LogDebug(VeloxConstants::LOGGING_TAG, "Adding Velox extension " + extension->GetName());
        extensions.push_back(extension);
    }

    bool VeloxExtensionService::HasExtension(std::shared_ptr<VeloxExtension> extension) const
    {
        return find(extensions.begin(), extensions.end(), extension) != extensions.end();
    }

    void VeloxExtensionService::RemoveExtension(std::shared_ptr<VeloxExtension> extension)
    {
        size_t n = extensions.size();
        extensions.erase(std::remove(extensions.begin(), extensions.end(), extension), extensions.end());
        if (n == extensions.size()) {
            LogWarning(VeloxConstants::LOGGING_TAG,
                       "Unable to remove Velox extension, extension unknown: '" + extension->GetName() + "'");
        } else {
            LogDebug(VeloxConstants::LOGGING_TAG, "Removed Velox extension " + extension->GetName());
        }
    }

    void VeloxExtensionService::InjectExtensions(velox::Interpreter &interpreter)
    {
        for (const auto &extension: extensions) {
            LogVerbose(VeloxConstants::LOGGING_TAG, "Injection Velox extension " + extension->GetName());
            extension->InjectExtension(interpreter);
        }
    }

}