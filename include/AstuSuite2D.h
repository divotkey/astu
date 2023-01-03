/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

/**
 * @file
 * @brief This header file includes all header files that make up the Suite2D module. 
 */

#pragma once

#include "Graphics/RalColors.h"
#include "Graphics/WebColors.h"
#include "Suite2D/PolygonVertexBuffer.h"
#include "Suite2D/Scene.h"
#include "Suite2D/CPose.h"
#include "Suite2D/CScene.h"
#include "Suite2D/CameraService.h"
#include "Suite2D/CameraControlService.h"
#include "Suite2D/LineRenderer.h"
#include "Suite2D/ShapeGenerator.h"

#include "Suite2D/SceneSystem.h"
#include "Suite2D/CAutoRotate.h"
#include "Suite2D/AutoRotateSystem.h"
#include "Suite2D/CBody.h"
#include "Suite2D/CColliders.h"
#include "Suite2D/PhysicsSystem.h"
#include "Suite2D/CollisionSignal.h"

namespace astu2d = astu::suite2d;

namespace astu {

    /**
     * @defgroup suite2d_group Suite 2D
     * 
     * @brief This module contains templates, classes, systems and components
     * dedicated to 2D game environments.
     * 
     * This module is relatively large, so here is another subdivision of the
     * contained element sorted by topics.
     * 
     * @section graph_sect Graphics
     * 
     * - astu::suite2d::Camera
     * - astu::suite2d::CameraService
     * - astu::suite2d::CameraControlService
     * - astu::suite2d::CameraClient
     * - astu::suite2d::LineRenderer
     * - astu::suite2d::SceneGraph
     * - astu::suite2d::SceneRenderer2D
     * - astu::suite2d::Spatial
     * - astu::suite2d::Node
     * - astu::suite2d::NodeBuilder
     * - astu::suite2d::Polyline
     * - astu::suite2d::PolylineBuilder
     * - astu::suite2d::SceneSystem
     * - astu::suite2d::CScene
     * - astu::suite2d::ShapeGenerator
     * 
     * @section physics_sect Physics
     * - astu::suite2d::PhysicsSystem
     * - astu::suite2d::CBody
     * - astu::suite2d::CBodyFactory
     * - astu::suite2d::CBodyBuilder
     * - astu::suite2d::CBodyCollider
     * - astu::suite2d::CBodyColliderBuilder
     * - astu::suite2d::CCircleCollider
     * - astu::suite2d::CCircleColliderFactory
     * - astu::suite2d::CCircleColliderBuilder
     * - astu::suite2d::CPolygonCollider
     * - astu::suite2d::CPolygonColliderFactory
     * - astu::suite2d::CPolygonColliderBuilder
     * - astu::suite2d::CollisionSignal
     * - astu::suite2d::CollisionListener
     * 
     * @section logic_sect Logic
     * - astu::suite2d::CPose
     * - astu::suite2d::AutoRotateSystem
     * - astu::suite2d::CAutoRotate
     *      
     * @{
     */

    /**@}*/

} // end of namespace
