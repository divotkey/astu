/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Suite2D/CommandService2D.h"

// C++ Standard Library includes
#include <iostream>

using namespace std;

namespace astu::suite2d {

    const std::string CommandService2D::LOGGING_TAG = "S2CMD";

    CommandService2D::CommandService2D(int updatePriority)
        : Service("Command Service 2D")
        , Updatable(updatePriority)
    {
        // Intentionally left empty.
    }

    void CommandService2D::OnStartup()
    {
        sceneGraph = ASTU_GET_SERVICE(SceneGraph);
        camSrv = ASTU_GET_SERVICE(CameraService);
        camCtrlSrv = ASTU_GET_SERVICE_OR_NULL(CameraControlService);
    }

    void CommandService2D::OnShutdown()
    {
        sceneGraph = nullptr;
        camCtrlSrv = nullptr;
        camSrv = nullptr;
        commands.Clear();
    }

    void CommandService2D::OnUpdate()
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        commands.Execute();
    }

    void CommandService2D::AttachToScene(std::shared_ptr<Spatial> spatial)
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        commands.Add([this, spatial]() {
            //LogDebug(LOGGING_TAG, "Attaching child spatial to root scene node");
            sceneGraph->GetRoot()->AttachChild(spatial);
        });
    }

    void CommandService2D::AttachToScene(std::shared_ptr<Spatial> spatial, const string &parentName)
    {
        commands.Add([this, spatial, parentName]() {
            auto parent = sceneGraph->GetRoot()->FindChildOrNull(parentName);
            if (!parent) {
                LogWarning(LOGGING_TAG, "Unable to attach spatial to scene, parent '" + parentName + "' is not found");
                return;
            }

            auto node = dynamic_pointer_cast<Node>(parent);
            if (!node) {
                LogWarning(LOGGING_TAG, "Unable to attach spatial to scene, parent '" + parentName + "' is not a node");
                return;
            }

            LogDebug(LOGGING_TAG, "Attaching spatial to scene node '" + parentName + "'");
            node->AttachChild(spatial);
        });
    }

    void CommandService2D::RemoveSpatial(const string &name)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        commands.Add([this, name]() {
            auto spatial = sceneGraph->GetRoot()->FindChildOrNull(name);
            if (!spatial) {
                return;
            }

            auto parent = spatial->GetParent();
            if (parent) {
                parent->DetachChild(spatial);
            }
        });

    }

    void CommandService2D::ClearScene()
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        commands.Add([this]() {
            LogDebug(LOGGING_TAG, "Clearing 2D scene");
            sceneGraph->GetRoot()->DetachAll();
        });
    }

    void CommandService2D::SetCameraPos(const Vector2f &pos)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (camCtrlSrv) {
            commands.Add([this, pos]() {
                camCtrlSrv->SetCameraPosition(pos);
            });
        } else {
            commands.Add([this, pos]() {
                camSrv->GetCamera()->SetPosition(pos);
            });
        }
    }

    void CommandService2D::SetCameraZoom(float zoomLevel)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (camCtrlSrv) {
            commands.Add([this, zoomLevel]() {
                camCtrlSrv->SetZoomLevel(zoomLevel);
            });
        } else {
            commands.Add([this, zoomLevel]() {
                camSrv->GetCamera()->SetZoom(CameraControlService::CalcZoom(zoomLevel));
            });
        }
    }

    void CommandService2D::SetCameraHomePos(const Vector2f &pos)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (camCtrlSrv) {
            commands.Add([this, pos]() {
                camCtrlSrv->SetHomePosition(pos);
            });
        }

    }

    void CommandService2D::SetCameraHomeZoom(float zoomLevel)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (camCtrlSrv) {
            commands.Add([this, zoomLevel]() {
                camCtrlSrv->SetHomeZoom(zoomLevel);
            });
        }
    }

    void CommandService2D::ShowFixedWidth(float width)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        commands.Add([this, width]() {
            GetCamera().ShowFixedWidth(width);
        });
    }

    void CommandService2D::ShowFixedHeight(float height)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        commands.Add([this, height]() {
            GetCamera().ShowFixedHeight(height);
        });
    }

    void CommandService2D::ShowScreenSpace()
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        commands.Add([this]() {
            GetCamera().ShowScreenSpace();
        });
    }

    void CommandService2D::ShowFitting(float width, float height)
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        commands.Add([this, width, height]() {
            GetCamera().ShowFitting(width, height);
        });
    }

    void CommandService2D::ShowFilling(float width, float height)
    {
        commands.Add([this, width, height]() {
            GetCamera().ShowFilling(width, height);
        });
    }

    void CommandService2D::ShowStretched(float width, float height)
    {
        commands.Add([this, width, height]() {
            GetCamera().ShowStretched(width, height);
        });
    }

    Camera& CommandService2D::GetCamera()
    {
        if (camCtrlSrv) {
            return camCtrlSrv->GetCamera();
        } else {
            return *camSrv->GetCamera();
        }
    }

} // end of namespace