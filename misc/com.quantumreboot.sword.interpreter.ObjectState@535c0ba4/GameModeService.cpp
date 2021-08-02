/*
 * Game Boilerplate
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "GameModeService.h"
#include "Constants.h"

using namespace astu2d;
using namespace astu;
using namespace std;

GameModeService::GameModeService()
  : BaseService("Game Mode Service")
{
	  // Intentionally left empty.    
}

void GameModeService::OnStartup()
{
  // Configure camera.
  GetCamera().ShowFitting(VIEW_WIDTH, VIEW_HEIGHT);

  // Add world boundaries.
  float vRadius = VIEW_HEIGHT / 2 + BOUNDARY_THICKNESS / 2;
  float hRadius = VIEW_WIDTH / 2 + BOUNDARY_THICKNESS / 2;
  AddEntity("HBoundary", 0, -vRadius);
  AddEntity("HBoundary", 0, vRadius);
  AddEntity("VBoundary", -hRadius, 0);
  AddEntity("VBoundary", hRadius, 0);

  // Add initial entities.
}

void GameModeService::OnShutdown()
{
  ASTU_SERVICE(EntityService).RemoveAll();
}

shared_ptr<Entity> GameModeService::AddEntity(const string& proto, float x, float y, float phiDeg)
{
  auto entity = ASTU_SERVICE(EntityFactoryService).CreateEntity(proto);
  auto& pose = entity->GetComponent<CPose>();
  pose.transform.SetTranslation(x, y);
  if (phiDeg != 0) {
      pose.transform.SetRotationDeg(phiDeg);
  }

  ASTU_SERVICE(EntityService).AddEntity(entity);
  return entity;
}
