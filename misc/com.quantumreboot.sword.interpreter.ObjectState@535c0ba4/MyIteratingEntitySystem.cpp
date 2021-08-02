/*
 * Game Boilerplate
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "MyIteratingEntitySystem.h"
#include "CMyComponent.h"

// AST Utilities includes
#include <AstuSuite2D.h>

using namespace astu2d;
using namespace astu;
using namespace std;

// TODO add types of entity components to the family e.g.,
// EntityFamily::Create<astu2d::CPose, astu2d::CBody>();

const EntityFamily MyIteratingEntitySystem::FAMILY = EntityFamily::Create<CMyComponent>();

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