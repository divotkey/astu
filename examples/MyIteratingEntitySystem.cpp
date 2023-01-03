/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2023. Roman Divotkey. All rights reserved.
 */

#include "MyIteratingEntitySystem.h"

using namespace astu;
using namespace std;

// TODO add types of entity components to the family e.g.,
// EntityFamily::Create<astu2d::CPose, astu2d::CBody>();

const EntityFamily MyIteratingEntitySystem::FAMILY = EntityFamily::Create<>();

MyIteratingEntitySystem::MyIteratingEntitySystem(int updatePriority)
    : BaseService("My Iterating-Entity System")
    , IteratingEntitySystem(FAMILY, updatePriority)    
{
    // Intentionally left empty.
}

void MyIteratingEntitySystem::OnStartup()
{
    // Intentionally left empty.
}

void MyIteratingEntitySystem::OnShutdown()
{
    // Intentionally left empty.
}

void MyIteratingEntitySystem::ProcessEntity(Entity & entity)
{
    // Process entity    
}