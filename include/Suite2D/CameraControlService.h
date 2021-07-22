#pragma once
/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Suite2D/CameraService.h"
#include "Service/UpdateService.h"
#include "Vector2.h"
#include "Events.h"

// C++ Standard Libraries
#include <string>

namespace astu::suite2d {

    /**
     * This service lets the user change the camera mode while the application 
     * is running.
     * 
     * @ingroup suite2d_group
     */
    class CameraControlService 
        : public BaseService
        , public Updatable
        , public KeystrokeListener
        , public MouseButtonListener
        , public MouseWheelListener
        , public CameraClient
    {
    public:

        /**
         * Constructor.
         * 
         * @param updatePriority the priority used to update this service
         */
        CameraControlService(int updatePriority = astu::Priority::Normal);

        // Inherited via BaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via IUpdatable
        virtual void OnUpdate() override;

        // Inherited via KeystrokeListener
        virtual bool OnKeyPressed(int keycode) override;

        // Inherited via MouseButtonListener
        virtual bool OnMouseButtonPressed(int button, int x, int y) override;
        virtual bool OnMouseButtonReleased(int button, int x, int y) override;

        // Inherited via MouseWheelListener
        virtual bool OnMouseWheel(int amount) override;

    private:
        /** The mouse button used to control the camera position. */
        int mouseButton;
        
        /** The key used to center the camera. */
        int homeKey;

        /** Indicates whether the user is currently changing the camera position. */
        bool dragging;

        /** Used to calculate the zoom factor. */
        int zoomLevel;

        /** The start position of the camera movement in screen coordinates. */
        astu::Vector2f startScreenPos;

        /** The start position of the camera movement in world coordinates. */
        astu::Vector2f startWorldPos;

        void UpdateCamera(int screenX, int screenY);

    };

} // end of namespace

