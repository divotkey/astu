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
         * Returns the current position of this camera.
         * 
         * @return the current position in world space
         */
        const astu::Vector2f GetPosition() const;

        /**
         * Sets the orientation of this camera
         * 
         * @param phi   the angle in radians
         * @return reference to this camera for method chaining
         */
        Camera2& SetOrientation(float phi);

        /**
         * Returns the current orientation of this camera.
         * 
         * @return the orientation in radians
         */
        float GetOrientation() const;

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

        /**
         * Switches the camera to screen space mode.
         * 
         * The camera shows the section of the world that is visible based on
         * the current resolution of the output window. Except for centering 
         * the output so that the coordinate origin is in the center of the
         * output window, the camera performs no transformation.         
         */
        Camera2& ShowScreenSpace();

        /**
         * Switches the camera to fixed width mode.
         * 
         * The camera will show the specified width of the game world
         * independently from the aspect ratio of the output window.
         * Empty areas on the top and bottom of the screen might appear.
         * 
         * @param width the visible width in world units
         */
        Camera2& ShowFixedWidth(float width);

        /**
         * Switches the camera to fixed height mode.
         * 
         * The camera will show the specified height of the game world
         * independently from the aspect ratio of the output window.
         * Empty areas on the left and right side of the screen might appear.
         * 
         * @param height    the visible height in world units
         */
        Camera2& ShowFixedHeight(float height);

        /**
         * Switches the camera to fitting view mode.
         * 
         * The camera will show the specified height of the game world
         * independently from the aspect ratio of the output window.
         * Empty areas on the top and bottom or on the left and right side 
         * of the screen might appear.
         * 
         * @param width     the visible width in world units
         * @param height    the visible height in world units
         */
        Camera2& ShowFitting(float width, float height);

        /**
         * Switches the camera to filling view mode.
         * 
         * The camera will show the specified height of the game world
         * independently from the aspect ratio of the output window.
         * If the aspect ratio of the output window does not match the
         * aspect ratio of the visible world area, parts of the game world
         * might not be cut off.
         * 
         * @param width     the visible width in world units
         * @param height    the visible height in world units
         */
        Camera2& ShowFilling(float width, float height);

        /**
         * Switches the camera to streched view mode.
         * 
         * In case aspect ratio of the output window does not match
         * the aspect ratio of the world dimensions, the output will
         * be streched.
         * 
         * @param width     the visible width in world units
         * @param height    the visible height in world units
         */
        Camera2& ShowStreched(float width, float height);

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
        const Matrix3f& GetMatrix() const;

        /**
         * Returns the inverse transformation matrix.
         * 
         * @return the inverse transformation matrix
         */
        const Matrix3f& GetInverseMatrix() const;


    private:
        /** The position of this camera in world space. */
        astu::Vector2f position;

        /** The scaling of this camera. */
        astu::Vector2f scaling;

        /** The orientation of this camera in radians. */
        float orientation;

        /** Indicates whether the transformation matrix requires update. */
        mutable bool dirty;

        /** Indicates whether the inverse transformation matrix requires update. */
        mutable bool invDirty;

        /** The transformation matrix of this camera. */
        mutable Matrix3f matrix;

        /** The inverse transformation matrix of this camera. */
        mutable Matrix3f invMatrix;

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
            /** The width do show in world units. */
            float worldWidth;
        };

        class FixedHeightState final : public CameraState {
        public:
            FixedHeightState(float height) : worldHeight(height) { }

            virtual void UpdateScaling(Camera2 & cam) override;

        private:
            /** The height do show in world units. */
            float worldHeight;
        };

        class FittingState final : public CameraState {
        public:
            FittingState(float w, float h) 
                : worldWidth(w), worldHeight(h), ar(w/h) { }

            virtual void UpdateScaling(Camera2 & cam) override;

        private:
            /** The width do show in world units. */
            float worldWidth;

            /** The height do show in world units. */
            float worldHeight;

            /** The aspect ratio of the visible world area. */
            float ar;
        };

        class FillingState final : public CameraState {
        public:
            FillingState(float w, float h) 
                : worldWidth(w), worldHeight(h), ar(w/h) { }

            virtual void UpdateScaling(Camera2 & cam) override;

        private:
            /** The width do show in world units. */
            float worldWidth;

            /** The height do show in world units. */
            float worldHeight;

            /** The aspect ratio of the visible world area. */
            float ar;
        };

        class StrechedState final : public CameraState {
        public:
            StrechedState(float w, float h) 
                : worldWidth(w), worldHeight(h) { }

            virtual void UpdateScaling(Camera2 & cam) override;

        private:
            /** The width do show in world units. */
            float worldWidth;

            /** The height do show in world units. */
            float worldHeight;
        };

        /** The current state of the camera. */
        std::unique_ptr<CameraState> state;

        void SwitchState(std::unique_ptr<CameraState> newState);

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

        std::shared_ptr<Camera2> GetOrCreateCamera(const std::string & name) {
            if (HasCamera(name)) {
                return GetCamera(name);
            } else {
                return CreateCamera(name);
            }
        }

        /**
         * Destroyes
         */
        void DestroyAll();

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via ReslizeListener
        virtual bool OnResize(int width, int height) override;

    private:
        std::unordered_map<std::string, std::shared_ptr<Camera2>> cameraMap;
    };

    class Camera2Client : public virtual Service {
    public:

        Camera2Client(const std::string & cameraName = Camera2Service::DEFAULT_CAMERA, bool createCamera = false) {
            AddStartupHook([this, cameraName, createCamera](){
                if (createCamera) {
                    camera = ASTU_SERVICE(Camera2Service).GetOrCreateCamera(cameraName);
                } else {
                    camera = ASTU_SERVICE(Camera2Service).GetCamera(cameraName);
                }
            });

            AddShutdownHook([this](){
                camera = nullptr;
            });
        }

        Camera2& GetCamera() {
            return *camera;
        }

        const Camera2& GetCamera() const {
            return *camera;
        }

    private:
        std::shared_ptr<Camera2> camera;
    };

} // end of namespace
