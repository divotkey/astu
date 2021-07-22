/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// Local includes
#include "EntityService.h"
#include "Math/Transform2.h"

namespace astu::suite2d {

    /**
     * An entity component which describes the position and orientation in
     * two-dimensional world space.
     * 
     * @ingroup suite2d_group
     */
    class CPose : public astu::EntityComponent {
    public:
        /** The transformation. */
        astu::Transform2<float> transform;

        /**
         * Constructor.
         */
        CPose() {
            transform.SetIdentity();
        }

        /**
         * Constructor.
         * 
         * @param x     the x-coordinate of the initial translation
         * @param y     the y-coordinate of the initial translation
         * @param phi   the angle of the initial rotation in radians
         */
        CPose(float x, float y, float phi = 0) {
            transform.SetTranslation(x, y);
            transform.SetRotation(phi);
        }

        // Inherited via EntityComponent
        virtual std::shared_ptr<EntityComponent> Clone() override {
            // Create copy using copy-constructor.
            return std::make_shared<CPose>(*this);
        }
    };

} // end of namespace

