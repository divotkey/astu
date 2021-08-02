/*
 * Game Boilerplate
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

#pragma once

// AST-Utilities includes
#include <AstuServices.h>
#include <AstuSuite2D.h>
#include <AstuECS.h>

class GameModeService 
  : public astu::BaseService
  , private astu2d::CameraClient
{
public:
		
	/**
	 * Constructor.
	 */
  GameModeService();

private:

  /**
   * Adds an entity to the game world.
   * 
   * @param proto     the name of the entity prototype
   * @param x         the x-coordinate of the spawn point
   * @param y         the y-coordinate of the spawn point
   * @param phiDeg    the orientation angle in degrees
   * @return the newly added entity
   */
  std::shared_ptr<astu::Entity> AddEntity(
      const std::string& proto, 
      float x, 
      float y, 
      float phiDeg = 0); 

  // Inherited via BaseService
  virtual void OnStartup() override;
  virtual void OnShutdown() override;
};
