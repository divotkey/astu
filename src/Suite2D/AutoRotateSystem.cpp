/*
 * Breakout Clone
 * Copyright (c) 2021 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Suite2D/AutoRotateSystem.h"
#include "Suite2D/CAutoRotate.h"
#include "Suite2D/CPose.h"

using namespace std;

namespace astu::suite2d {

    const EntityFamily AutoRotateSystem::FAMILY = EntityFamily::Create<CPose, CAutoRotate>();

    AutoRotateSystem::AutoRotateSystem(int updatePriority)
        : BaseService("Auto-Rotate Entity System")
        , OneFamilyEntitySystem(FAMILY)
        , Updatable(updatePriority)
    {
        // Intentionally left empty.
    }

    void AutoRotateSystem::OnUpdate()
    {
        ProcessEntities();
    }

    void AutoRotateSystem::ProcessEntity(astu::Entity & entity)
    {
        auto& pose = entity.GetComponent<CPose>();
        auto& autoRotate = entity.GetComponent<CAutoRotate>();
        pose.transform.Rotate( autoRotate.speed * GetElapsedTimeF() );
    }

} // end of namespace