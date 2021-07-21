/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <iostream>

// Local includes
#include "Suite2D/CPose.h"
#include "Suite2D/CScene.h"
#include "Suite2D/SceneSystem.h"

using namespace std;

namespace astu::suite2d {

    const EntityFamily SceneSystem::FAMILY = EntityFamily::Create<CPose, CScene>();

    SceneSystem::SceneSystem(int updatePriority)
        : Service("2D Scene Entity System")
        , Updatable(updatePriority)
        , OneFamilyEntitySystem(FAMILY)
        , EntityListener(FAMILY)
    {
        // Intentionally left empty
    }

    void SceneSystem::OnStartup()
    {
        // TODO support configurable root elements.
        root = ASTU_SERVICE(SceneGraph2D).GetRoot();
    }

    void SceneSystem::OnShutdown()
    {
        root = nullptr;
    }

    void SceneSystem::OnUpdate()
    {
        ProcessEntities();
    }

    void SceneSystem::ProcessEntity(Entity & entity)
    {
        auto& pose = entity.GetComponent<CPose>();
        auto& scene = entity.GetComponent<CScene>();

        scene.spatial->SetLocalTransform(pose.transform);
    }

    void SceneSystem::OnEntityAdded(std::shared_ptr<Entity> entity)
    {
        auto& pose = entity->GetComponent<CPose>();
        auto& scene = entity->GetComponent<CScene>();

        scene.spatial->SetLocalTransform(pose.transform);
        root->AttachChild(scene.spatial);
    }

    void SceneSystem::OnEntityRemoved(std::shared_ptr<Entity> entity)
    {
        auto& scene = entity->GetComponent<CScene>();
        root->DetachChild(scene.spatial);
    }

} // end of namespace