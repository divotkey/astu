/*
 * Game Boilerplate
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "EntityPrototypeService.h"
#include "Constants.h"

// AST-Utilities includes
#include <AstuSuite2D.h>

using namespace astu2d;
using namespace astu;
using namespace std;

EntityPrototypeService::EntityPrototypeService(bool debug)
  : BaseService("Entity Prototype Service")
  , debug(debug)
{
	// Intentionally left empty. 
}

void EntityPrototypeService::OnStartup()
{
	// Create and register entity prototypes.
	auto &entityFactory = ASTU_SERVICE(EntityFactoryService);

  // Register boundary entity prototypes.
  entityFactory.RegisterPrototype("HBoundary", CreateBoundaryPrototype(VIEW_WIDTH, BOUNDARY_THICKNESS));
  entityFactory.RegisterPrototype("VBoundary", CreateBoundaryPrototype(BOUNDARY_THICKNESS, VIEW_HEIGHT));

  // TODO register entity prototypes.
}

void EntityPrototypeService::OnShutdown()
{
  // Cleanup.
  ASTU_SERVICE(EntityFactoryService).DeregisterAllPrototypes();
}

shared_ptr<Entity> EntityPrototypeService::CreateBoundaryPrototype(float w, float h)
{
  auto entity = make_shared<Entity>();
  entity->AddComponent(make_shared<CPose>());

  if (debug) {
      entity->AddComponent(make_shared<CScene>(PolylineBuilder()
          .Color(BOUNDARY_COLOR)
          .VertexBuffer(ShapeGenerator().GenRectangle(w, h))
          .Build()));
  }

  entity->AddComponent(CBodyBuilder()
      .Type(CBody::Type::Static)
      .Build());

  entity->AddComponent(CPolygonColliderBuilder()
      .Polygon(ShapeGenerator().GenRectangle(w, h))
      .Friction(0)
      .Restitution(1)
      .Build());

  return entity;
}