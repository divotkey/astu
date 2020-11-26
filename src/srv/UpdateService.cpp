/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <algorithm>
#include "ServiceManager.h"
#include "UpdateService.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// UpdateService
    /////////////////////////////////////////////////

    UpdateService::UpdateService()
        : BaseService("UpdateService")
    {
        // Intentionally left empty.
    }

    void UpdateService::AddUpdatable(std::shared_ptr<IUpdatable> updatable)
    {
        lstMngr.AddListener(updatable, updatable->GetUpdatePriority());
    }

    void UpdateService::RemoveUpdatable(std::shared_ptr<IUpdatable> updatable)
    {
        lstMngr.RemoveListener(updatable);
    }

    bool UpdateService::HasUpdatable(std::shared_ptr<IUpdatable> updatable)
    {
        return lstMngr.HasListener(updatable);
    }

    void UpdateService::UpdateAll()
    {
        lstMngr.VisitListeners([](IUpdatable & updatable) { 
            updatable.OnUpdate(); 
        });
    }

    /////////////////////////////////////////////////
    /////// UpdatableBaseService
    /////////////////////////////////////////////////

    UpdatableBaseService::UpdatableBaseService(const std::string & name, int priority)
        : BaseService(name)
        , updatePriority(priority)
    {
        // Intentionally left empty.        
    }

    void UpdatableBaseService::Startup() {
        GetSM().GetService<UpdateService>().AddUpdatable(shared_as<IUpdatable>());
        BaseService::Startup();
    }

    void UpdatableBaseService::Shutdown() {
        BaseService::Shutdown();
        GetSM().GetService<UpdateService>().RemoveUpdatable(shared_as<IUpdatable>());
    }

    int UpdatableBaseService::GetUpdatePriority() const
    {
        return updatePriority;
    }

}