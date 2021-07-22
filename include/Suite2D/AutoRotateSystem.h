/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "EntitySystems.h"
#include "Service/UpdateService.h"
#include "Service/TimeService.h"

namespace astu::suite2d {

    /**
     * This auto-rotate system rotates entities with a constant speed.
     * This system relies on CPose and CAutoRotate components.
     * 
     * @ingroup suite2d_group
     */
    class AutoRotateSystem 
        : public BaseService
        , private OneFamilyEntitySystem
        , private Updatable
        , private TimeClient
    {
    public:

        /**
         * Constructor.
         * 
         * @param updatePriority the priority to update this system
         */
        AutoRotateSystem(int updatePriority = astu::Priority::Normal);

    private:
        /** Constant defining the entites this system processes. */
        static const astu::EntityFamily FAMILY;

        // Inherited via  OneFamilyEntitySystem
        virtual void ProcessEntity(astu::Entity & entity) override;

        // Inherited via Updatable
        virtual void OnUpdate() override;
    };

} // end of namespace