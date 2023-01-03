/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ECS/EntityService.h"

namespace astu::suite2d {

    /**
     * Auto-Rotate components make entities rotate with a certain speed.
     * 
     * @ingroup suite2d_group
     */
    class CAutoRotate : public astu::EntityComponent {
    public: 
        /** The angular speed of this auto rotate component in radians/second. */
        float speed;

        /**
         * Constructor.
         * 
         * @param speed the angular speed in radians per second
         */
        CAutoRotate(float speed) : speed(speed) {
            // Intentionally left empty.        
        }

        // Inherited via EntityComponent
        virtual std::shared_ptr<EntityComponent> Clone() override {
            // Create copy using copy-constructor.
            return std::make_shared<CAutoRotate>(*this);
        }    
    };

} // end of namespace