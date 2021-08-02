/*
 * Game Boilerplate
 * 
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "EntityPrototypeService.h"	
#include "GameModeService.h"

// ASTU/Box2D includes
#include <AstuBox2D.h>

// AST-Utilities includes
#include <AstuSuiteSDL.h>
#include <AstuInput.h>
#include <AstuECS.h>
#include <AstuSuite2D.h>

using namespace astu2d;
using namespace astu;
using namespace std;

// Main entry point of the application.
int main()
{
	SdlApplication app;

	// Cameras do the world-to-screen transform and used by the scene graph.
	ASTU_CREATE_AND_ADD_SERVICE(CameraService);
	ASTU_CREATE_AND_ADD_SERVICE(CameraControlService);

	// Add Services and Systems required ECS.
	ASTU_CREATE_AND_ADD_SERVICE(EntityService);
	ASTU_CREATE_AND_ADD_SERVICE(EntityFactoryService);
	ASTU_CREATE_AND_ADD_SERVICE(Box2DPhysicsSystem);
	ASTU_CREATE_AND_ADD_SERVICE(CollisionSignalService);
	ASTU_CREATE_AND_ADD_SERVICE(SceneSystem);

	// Let's add our main service that runs the application.
	ASTU_CREATE_AND_ADD_SERVICE(EntityPrototypeService);
	ASTU_CREATE_AND_ADD_SERVICE(GameModeService);

	// Configure physics environment.
	ASTU_SERVICE(PhysicsSystem).SetGravityVector(0, 0);

	// Configure input controls (Axis and Actions)
	// ASTU_SERVICE(InputMappingService).AddAxisMapping("Paddle", Keys::Left, -1.0f);
	// ASTU_SERVICE(InputMappingService).AddAxisMapping("Paddle", Keys::Right, 1.0f);
	// ASTU_SERVICE(InputMappingService).AddAxisMapping("Paddle", Keys::GamepadLeftThumbstickX);

	// Configure application.
	app.SetApplicationName("Game Boilerplate");
	app.SetVersionString("1.0.0");
	app.SetResolution(Resolution::WXGA);
	app.SetFullscreen(false);

	// Run the application.
	return app.Run();
} 
