/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// AST Utilities includes
#include <AstuECS.h>

class MyIteratingEntitySystem 
    : public astu::BaseService
    , private astu::IteratingEntitySystem
{
public:

    // Constructor.
    MyIteratingEntitySystem(int updatePriority = astu::Priority::Normal);

private:
    // The family of entities this system processes
    static const astu::EntityFamily FAMILY;

    // Inherited via BaseService
    virtual void OnStartup() override;
    virtual void OnShutdown() override;

    // Inherited via IteratingEntitySystem
    virtual void ProcessEntity(astu::Entity& entity) override;
};