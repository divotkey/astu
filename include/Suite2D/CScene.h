/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ECS/EntityService.h"
#include "Scene.h"

// C++ Standard Library includes
#include <memory>

namespace astu::suite2d {

    /**
     * A component containing a spacial 2D scene graph element.
     * 
     * This type of component is typically used for visual representation
     * of entities in two-dimensional worlds.
     * 
     * @ingroup suite2d_group
     */
    class CScene : public astu::EntityComponent {
    public:
        /** Branch of the scene graph. */
        std::shared_ptr<Spatial> spatial;

        /**
         * Constructor.
         * 
         * @param spatial   the scene node branch representing the entity
         */
        CScene(std::shared_ptr<Spatial> spatial)
            : spatial(spatial)
        {
            // Intentionally left empty.
        }

        // Inherited via EntityComponent
        virtual std::shared_ptr<astu::EntityComponent> Clone() override {
            // We must create a deep copy of the branch of scene graph. */
            return std::make_shared<CScene>( spatial->Clone() );
        }
    };

} // end of namespace