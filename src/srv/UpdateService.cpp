/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <algorithm>
#include "UpdateService.h"

namespace astu {

    bool compare(std::shared_ptr<IUpdatable> & u1, std::shared_ptr<IUpdatable> & u2) {
        return u1->GetUpdatePriority() < u2->GetUpdatePriority();
    }

    UpdateService::UpdateService()
        : BaseService("UpdateService")
    {
        // Intentionally left empty.
    }

    void UpdateService::AddUpdatable(std::shared_ptr<IUpdatable> updatable)
    {
        if (!HasUpdatable(updatable)) {
            updatables.push_back(updatable);
        }

        std::sort(updatables.begin(), updatables.end(), compare);
    }

    void UpdateService::RemoveUpdatable(std::shared_ptr<IUpdatable> updatable)
    {
        updatables.erase(
                std::remove(updatables.begin(), updatables.end(), updatable), 
                updatables.end()
            );        
    }

    bool UpdateService::HasUpdatable(std::shared_ptr<IUpdatable> updatable)
    {
        return std::find(updatables.begin(), updatables.end(), updatable) != updatables.end();       
    }

    void UpdateService::Update()
    {
        for (auto & updatable : updatables) {
            updatable->OnUpdate();
        }
    }

}