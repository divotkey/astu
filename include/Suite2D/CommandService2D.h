/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/UpdateService.h"
#include "Service/LoggingService.h"
#include "Util/CommandQueue.h"
#include "Suite2D/Scene.h"
#include "Suite2D/CameraControlService.h"
#include "Suite2D/CameraService.h"

// C++ Standard Libraries
#include <string>
#include <mutex>

namespace astu::suite2d {

    /**
     * This service represents a thread-save way to issue commands dealing with the Suite2D facility.
     * 
     * @ingroup suite2d_group
     */
    class CommandService2D
        : public BaseService
        , public Updatable
        , public LoggingClient
    {
    public:

        /** Used to identify logging messages. */
        static const std::string LOGGING_TAG;

        /**
         * Constructor.
         * 
         * @param updatePriority the priority used to update this service
         */
        CommandService2D(int updatePriority = astu::Priority::Normal);

        void AttachToScene(std::shared_ptr<Spatial> spatial);
        void AttachToScene(std::shared_ptr<Spatial> spatial, const std::string &parent);
        void RemoveSpatial(const std::string &name);
        void ClearScene();
        void SetCameraPos(const Vector2f &pos);
        void SetCameraZoom(float zoomLevel);
        void SetCameraHomeZoom(float zoomLevel);
        void SetCameraHomePos(const Vector2f &pos);

        void ShowFixedWidth(float width);
        void ShowFixedHeight(float height);
        void ShowFitting(float width, float height);
        void ShowFilling(float width, float height);
        void ShowStretched(float width, float height);
        void ShowScreenSpace();

    protected:
        // Inherited via BaseService
        virtual void OnStartup() override;
        virtual void OnShutdown() override;

        // Inherited via IUpdatable
        virtual void OnUpdate() override;

    private:
        /** Used to issue scene graph related commands. */
        std::shared_ptr<SceneGraph> sceneGraph;

        /** Used to control camera position and zoom level. */
        std::shared_ptr<CameraControlService> camCtrlSrv;

        /** Used to handle cameras directly. */
        std::shared_ptr<CameraService> camSrv;

        /** Used to synchronize access to command queue. */
        std::mutex queueMutex;

        /** Stores the queued commands. */
        CommandQueue commands;


        Camera& GetCamera();
    };

} // end of namespace

