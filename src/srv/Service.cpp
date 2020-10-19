/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <stdexcept>
#include "Service.h"

namespace astu {

BaseService::BaseService(const std::string & aName)
    : name(aName)
    , running(false)
{
    // Intentionally left empty
}

const std::string & BaseService::GetName() const
{
    return name;
}

void BaseService::startup()
{
    if (isRunning()) {
        throw std::logic_error("Service " + GetName() + " already running");
    }
    onStartup();    
}

void BaseService::shutdown()
{
    // Best practice: ignore shut down calls on not-running services.
    if (isRunning()) {
        onShutdown();        
    }
}

bool BaseService::isRunning() const
{
    return running;
}


} // end of namespace