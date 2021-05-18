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
#include "MathUtils.h"
#include "Service.h"
#include "Matrix3.h"
#include "Events.h"

namespace astu {

    class Camera2 {
    public:

        /**
         * Constructor.
         */
        Camera2();

        /**
         * Sets the position of this camera.
         * 
         * @param x the x-coordinate of the position in world space
         * @param y the y-coordinate of the position in world space
         * @return reference to this camera for method chaining
         */
        Camera2& SetPosition(float x, float y);

        /**
         * Sets the position of this camera.
         * 
         * @param p the positoin in world space
         * @return reference to this camera for method chaining
         */
        Camera2& SetPosition(const Vector2f & p) {
            return SetPosition(p.x, p.y);
        }

        /**
         * Sets the orientation of this camera
         * 
         * @param phi   the angle in radians
         * @return reference to this camera for method chaining
         */
        Camera2& SetOrientation(float phi);

        /**
         * Sets the orientation of this camera
         * 
         * @param phi   the angle in degree
         * @return reference to this camera for method chaining
         */
        Camera2& SetOrientationDeg(float phi) {
            orientation =  MathUtils::ToRadians(phi);
            return *this;
        }

        Camera2& ShowScreenSpace();

        Camera2& ShowFixedWorldWidth(float w);

        Camera2& ShowFixedWorldHeight(float h);

        /**
         * Resets this camera to its default configuration.
         * 
         * @return reference to this camera for method chaining
         */
        Camera2& Reset();

        /**
         * Returns the transformation matrix.
         * 
         * @return the transformation matrix
         */
        const Matrix3f& GetTransform() const;

    private:
        /** The position of this camera in world space. */
        astu::Vector2f position;

        /** The scaling of this camera. */
        astu::Vector2f scaling;

        /** The orientation of this camera in radians. */
        float orientation;



        /** Indicates whether the transformation matrix requires update. */
        mutable bool dirty;

        /** The transformation matrix of this camera. */
        mutable Matrix3f matrix;

        /** The width of the render target. */
        float targetWidth;

        /** The height of the render target. */
        float targetHeight;

        /**
         * Sets the size of the render target.
         * 
         * @param width     the width in pixel
         * @param height    the height in pixel
         */
        void SetRenderTargetSize(int width, int height);

        // Internal States
        class CameraState {
        public:
            virtual ~CameraState() {}
            virtual void UpdateScaling(Camera2 & cam) = 0;
        };

        class ScreenSpaceState final : public CameraState {
        public:
            virtual void UpdateScaling(Camera2 & cam) override;
        };

        class FixedWidthState final : public CameraState {
        public:
            FixedWidthState(float width) : worldWidth(width) { }

            virtual void UpdateScaling(Camera2 & cam) override;

        private:
            /** The wirth do show in world units. */
            float worldWidth;
        };

        class FixedHeightState final : public CameraState {
        public:
            FixedHeightState(float height) : worldHeight(height) { }

            virtual void UpdateScaling(Camera2 & cam) override;

        private:
            /** The wirth do show in world units. */
            float worldHeight;
        };


        /** The current state of the camera. */
        std::unique_ptr<CameraState> state;

        friend class Camera2Service;
    };

    class Camera2Service final 
        : public virtual Service
        , public ReslizeListener
    {
    public:

        /** The name of the default camera. */
        static const std::string DEFAULT_CAMERA;

        /**
         * Constructor.
         */
        Camera2Service();

        std::shared_ptr<Camera2> CreateCamera(const std::string & name);

        std::shared_ptr<Camera2> GetCamera(const std::string & name = DEFAULT_CAMERA);

        bool HasCamera(const std::string & name) const;

        /**
         * Destroyes
         */
        void DestroyAll();

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via ReslizeListener
        virtual void OnResize(int width, int height) override;

    private:
        std::unordered_map<std::string, std::shared_ptr<Camera2>> cameraMap;
    };

} // end of namespace
