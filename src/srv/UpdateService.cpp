/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes.
#include "UpdateService.h"

namespace astu {

    UpdateService::UpdateService()
        : Service("Update Service")
    {
        // Intentionally left empty.        
    }

    void UpdateService::AddUpdatable(IUpdatable & updatable, int priority)
    {
        lstMngr.AddListener(&updatable, priority);
    }

    void UpdateService::RemoveUpdatable(IUpdatable & updatable)
    {
        lstMngr.RemoveListener(&updatable);
    }

    bool UpdateService::HasUpdatable(IUpdatable & updatable) const 
    {
        return lstMngr.HasListener(&updatable);
    }

    int UpdateService::GetUpdatePriority(IUpdatable & updatable) const
    {
        if (!lstMngr.HasListener(&updatable) ) {
            throw std::logic_error(
                "Unable to retrieve update priority, specified updatable is unknown");
        }

        return lstMngr.GetListenerPriority(&updatable);
    }

    void UpdateService::UpdateAll()
    {
        lstMngr.VisitListeners([](IUpdatable & updatable) { 
            updatable.OnUpdate(); 
        });
    }

    /////////////////////////////////////////////////
    /////// Updatable
    /////////////////////////////////////////////////

    Updatable::Updatable(int priority)
    {
       AddStartupHook([this, priority]() { ASTU_SERVICE(UpdateService).AddUpdatable(*this, priority); } );
        AddShutdownHook([this]() { ASTU_SERVICE(UpdateService).RemoveUpdatable(*this); } );
    }

} // end of namespace
