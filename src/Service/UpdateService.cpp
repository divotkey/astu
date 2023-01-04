/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes.
#include "Service/UpdateService.h"

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
            return false;
        });
    }

    /////////////////////////////////////////////////
    /////// Updatable
    /////////////////////////////////////////////////

    Updatable::Updatable(int priority)
        : updatePriority(priority)
    {
        AddStartupHook([this, priority]() { 
            ASTU_SERVICE(UpdateService).AddUpdatable(*this, priority); } );

        AddShutdownHook([this]() { 
            ASTU_SERVICE(UpdateService).RemoveUpdatable(*this); } );

        AddPauseHook([this]() { 
            if (GetStatus() == Running) {
                ASTU_SERVICE(UpdateService).RemoveUpdatable(*this);
            }            
        });

        AddResumeHook([this]() { 
            if (GetStatus() == Paused) {
                ASTU_SERVICE(UpdateService).AddUpdatable(*this, updatePriority);
            }
        });
    }

} // end of namespace
