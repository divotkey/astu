/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ECS/EntityService.h"
#include "AstuECS.h"

namespace astu {

    /**
     * This entity component describes when the entity should be removed.
     * 
     * @ingroup ecs_group
     */
    class CAutoDestruct : public EntityComponent {
    public:
        // Duration of the entity to live.
        float duration;

        // Constructor.
        CAutoDestruct(float duration) : duration(duration) {
            // Intentionally left empty.        
        }

        // Inherited via EntityComponent
        virtual std::shared_ptr<EntityComponent> Clone() override {
            // Create copy using copy-constructor.
            return std::make_shared<CAutoDestruct>(*this);
        }    
    };

} // end of namespace

