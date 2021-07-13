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
#include "Vector2.h"
#include "Matrix3.h"
#include "Events.h"

namespace astu {

    /////////////////////////////////////////////////
    /////// Camera2D
    /////////////////////////////////////////////////

    /**
     * A camera object describes the portion of the game world shown on the
     * screen. This camera object ist used for two-dimensional
     * game environments, hence the name `Camera2D`. 
     */
    class Camera2D {
    public:

        /**
         * Constructor.
         */
        Camera2D();

        /**
         * Sets the position of this camera.
         * 
         * @param x the x-coordinate of the position in world space
         * @param y the y-coordinate of the position in world space
         * @return reference to this camera for method chaining
         */
        Camera2D& SetPosition(float x, float y);

        /**
         * Sets the position of this camera.
         * 
         * @param p the positoin in world space
         * @return reference to this camera for method chaining
         */
        Camera2D& SetPosition(const Vector2f & p) {
            return SetPosition(p.x, p.y);
        }

        /**
         * Sets the zoom factor of this camera.
         * 
         * @param z the zoom factor
         * @return reference to this camera for method chaining
         */
        Camera2D& SetZoom(float z);

        /**
         * Returns the current zoom factor of this camera.
         * 
         * @return the zoom factor
         */
        float GetZoom() const;

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
        Camera2D& SetOrientation(float phi);

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
        Camera2D& SetOrientationDeg(float phi) {
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
        Camera2D& ShowScreenSpace();

        /**
         * Switches the camera to fixed width mode.
         * 
         * The camera will show the specified width of the game world
         * independently from the aspect ratio of the output window.
         * Empty areas on the top and bottom of the screen might appear.
         * 
         * @param width the visible width in world units
         */
        Camera2D& ShowFixedWidth(float width);

        /**
         * Switches the camera to fixed height mode.
         * 
         * The camera will show the specified height of the game world
         * independently from the aspect ratio of the output window.
         * Empty areas on the left and right side of the screen might appear.
         * 
         * @param height    the visible height in world units
         */
        Camera2D& ShowFixedHeight(float height);

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
        Camera2D& ShowFitting(float width, float height);

        /**
         * Switches the camera to fitting view mode.
         * 
         * The camera will show the specified height of the game world
         * independently from the aspect ratio of the output window.
         * Empty areas on the top and bottom or on the left and right side 
         * of the screen might appear.
         * 
         * @param size  vector containing width and height in world units
         */
        Camera2D& ShowFitting(const astu::Vector2f size) {
            return ShowFitting(size.x, size.y);
        }

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
        Camera2D& ShowFilling(float width, float height);

        /**
         * Switches the camera to filling view mode.
         * 
         * The camera will show the specified height of the game world
         * independently from the aspect ratio of the output window.
         * If the aspect ratio of the output window does not match the
         * aspect ratio of the visible world area, parts of the game world
         * might not be cut off.
         * 
         * @param size  vector containing width and height in world units
         */
        Camera2D& ShowFilling(const astu::Vector2f size) {
            return ShowFilling(size.x, size.y);
        }

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
        Camera2D& ShowStreched(float width, float height);

        /**
         * Switches the camera to streched view mode.
         * 
         * In case aspect ratio of the output window does not match
         * the aspect ratio of the world dimensions, the output will
         * be streched.
         * 
         * @param size  vector containing width and height in world units
         */
        Camera2D& ShowStreched(const astu::Vector2f size) {
            return ShowStreched(size.x, size.y);
        }

        /**
         * Resets this camera to its default configuration.
         * 
         * @return reference to this camera for method chaining
         */
        Camera2D& Reset();

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

        /** The zoom factor used additionally to the scaling. */
        float zoom;

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
            virtual void UpdateScaling(Camera2D & cam) = 0;
        };

        class ScreenSpaceState final : public CameraState {
        public:
            virtual void UpdateScaling(Camera2D & cam) override;
        };

        class FixedWidthState final : public CameraState {
        public:
            FixedWidthState(float width) : worldWidth(width) { }

            virtual void UpdateScaling(Camera2D & cam) override;

        private:
            /** The width do show in world units. */
            float worldWidth;
        };

        class FixedHeightState final : public CameraState {
        public:
            FixedHeightState(float height) : worldHeight(height) { }

            virtual void UpdateScaling(Camera2D & cam) override;

        private:
            /** The height do show in world units. */
            float worldHeight;
        };

        class FittingState final : public CameraState {
        public:
            FittingState(float w, float h) 
                : worldWidth(w), worldHeight(h), ar(w/h) { }

            virtual void UpdateScaling(Camera2D & cam) override;

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

            virtual void UpdateScaling(Camera2D & cam) override;

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

            virtual void UpdateScaling(Camera2D & cam) override;

        private:
            /** The width do show in world units. */
            float worldWidth;

            /** The height do show in world units. */
            float worldHeight;
        };

        /** The current state of the camera. */
        std::unique_ptr<CameraState> state;

        void SwitchState(std::unique_ptr<CameraState> newState);

        friend class CameraService2D;
    };

    /////////////////////////////////////////////////
    /////// CameraService2D
    /////////////////////////////////////////////////

    /**
     * The camera service manages various camera objects. Cameras are created
     * with a specific name and can be retrieved using this name.
     */
    class CameraService2D final 
        : public virtual Service
        , public ResizeListener
    {
    public:

        /** The name of the default camera. */
        static const std::string DEFAULT_CAMERA;

        /**
         * Constructor.
         */
        CameraService2D();

        /**
         * Creates a new camera with the specified name.
         * 
         * @param camName   the name of the camera to create
         * @return the newly created camera
         * @throws std::logic_error in case a camera with that name already
         *  exists
         */
        std::shared_ptr<Camera2D> CreateCamera(const std::string & camName);

        /**
         * Retrieves the camera with the specified name.
         * 
         * @param camName  the name of the camera
         * @return the requested camera
         * @throws std::logic_error in case the camera is unknown
         */
        std::shared_ptr<Camera2D> 
            GetCamera(const std::string & camName = DEFAULT_CAMERA);

        /**
         * Tests whether a camera with the specified name exists.
         * 
         * @param camName  the name of the camera
         * @return `true` if the camera exists
         */
        bool HasCamera(const std::string & camName) const;

        /**
         * Retrieves a camera with the specified name or creates it.
         * 
         * @param name  the name of the camera
         * @return the retrieved or newly created camera
         */
        std::shared_ptr<Camera2D> GetOrCreateCamera(const std::string & camName) {
            if (HasCamera(camName)) {
                return GetCamera(camName);
            } else {
                return CreateCamera(camName);
            }
        }

        /**
         * Destroys all cameras.
         */
        void DestroyAll();

        // Inherited via Service
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via ReslizeListener
        virtual bool OnResize(int width, int height) override;

    private:
        /** Associates the cameras with names. */
        std::unordered_map<std::string, std::shared_ptr<Camera2D>> cameraMap;
    };

    /////////////////////////////////////////////////
    /////// CameraClient2D
    /////////////////////////////////////////////////

    class CameraClient2D : public virtual Service {
    public:

        /**
         * Constructor.
         * 
         * @param cameraName    the name of the camera to be used
         * @param createCamera  whether the camera should be created
         */
        CameraClient2D(
            const std::string & cameraName = CameraService2D::DEFAULT_CAMERA, 
            bool createCamera = false);

        /**
         * Returne the camera this client uses.
         * 
         * @return the used camera
         */
        Camera2D& GetCamera() {
            return *camera;
        }

        /**
         * Returne the camera this client uses.
         * 
         * @return the used camera
         */
        const Camera2D& GetCamera() const {
            return *camera;
        }

        /**
         * Specified which camera to use.
         * 
         * @param camName   the name of the camera
         * @param create    whether the camera should be created
         * @throws std::logic_error in case the camera is unknown
         */
        void UseCamera(const std::string & camName, bool create = false);

        /** 
         * Returns the name of the camera to be used.
         * 
         * 
         * @return the name of the camera
         */
        const std::string& GetCameraName() const;

    private:
        /** Determines whether the used camera should be created. */
        bool createCamera;

        /** The name of the camera to use. */
        std::string cameraName;

        /** The camera. */
        std::shared_ptr<Camera2D> camera;

        /** 
         * Fetches or create the requested camera.
         */
        void InitCamera();
    };

} // end of namespace
