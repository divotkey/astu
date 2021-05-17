/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard includes
#include <unordered_map>
#include <memory>
#include <string>

// Local includes
#include "Service.h"
#include "Matrix3.h"

namespace astu {

    class Camera2 {
    public:

        void SetPosition(float x, float y);

        void SetPosition(const Vector2f& p) {
            SetPosition(p.x, p.y);
        }


        /**
         * Returns the transformation matrix.
         * 
         * @return the transformation matrix
         */
        const Matrix3f GetTransform();

    private:
        /** The transformation matrix. */
        Matrix3f transform;
    };

    class Camera2Manager : virtual public Service {
    public:

        /** The name of the default camera. */
        static const std::string DEFAULT_CAMERA;

        /**
         * Constructor.
         */
        Camera2Manager();

        std::shared_ptr<Camera2> CreateCamera(const std::string & name = DEFAULT_CAMERA);

        std::shared_ptr<Camera2> GetCamera(const std::string & name);

        bool HasCamera(const std::string & name) const;

        /**
         * Destroyes
         */
        void DestroyAll();

    private:
        std::unordered_map<std::string, std::shared_ptr<Camera2>> cameraMap;
    };

} // end of namespace
