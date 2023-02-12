/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Suite2D/CameraControlService.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <cmath>
using namespace std;

namespace astu::suite2d {

    const float CameraControlService::ZoomStep = 1.1f;

    CameraControlService::CameraControlService(int updatePriority)
        : Service("Camera 2D Control Service")
        , Updatable(updatePriority)
        , mouseButton(Mouse::Button::RIGHT)
        , homeKey(Keyboard::KEY_HOME)
        , homePos(Vector2f::Zero)
        , dragging(false)
        , zoomLevel(0)
        , homeZoom(0)
    {
        // Intentionally left empty.
    }

    void CameraControlService::OnStartup()
    {
        dragging = false;
        zoomLevel = homeZoom;
    }

    void CameraControlService::OnShutdown()
    {
        // Intentionally left empty.
    }

    void CameraControlService::OnUpdate()
    {
        if (!dragging) {
            return;
        }

        Mouse mouse;
        UpdateCamera(mouse.GetCursorX(), mouse.GetCursorY());
    }

    bool CameraControlService::OnKeyPressed(int keycode)
    {
        if (keycode == homeKey) {
            GetCamera().SetPosition(homePos);
            GetCamera().SetZoom(CalcZoom(homeZoom));
            zoomLevel = homeZoom;
            dragging = false;
            return true;
        }
        return false;
    }

    bool CameraControlService::OnMouseButtonPressed(int button, int x, int y)
    {
        if (button != mouseButton) {
            return false;
        }

        startScreenPos.Set(static_cast<float>(x), static_cast<float>(y));
        startWorldPos = GetCamera().GetPosition();
        dragging = true;

        return true;
    }

    bool CameraControlService::OnMouseButtonReleased(int button, int x, int y)
    {
        if (button != mouseButton || !dragging) {
            return false;
        }

        UpdateCamera(x, y);
        dragging = false;
        return true;
    }

    bool CameraControlService::OnMouseWheel(int amount)
    {
        zoomLevel += static_cast<float>(amount);
        GetCamera().SetZoom( CalcZoom(zoomLevel) );
        return true;
    }

    void CameraControlService::UpdateCamera(int screenX, int screenY)
    {
        auto curScreenPos 
            = Vector2f(static_cast<float>(screenX), static_cast<float>(screenY));

        auto deltaWorld = GetCamera()
            .GetInverseMatrix()
            .TransformVector(startScreenPos - curScreenPos);

        GetCamera().SetPosition(startWorldPos + deltaWorld);
    }

    void CameraControlService::SetHomePosition(const Vector2f pos)
    {
        homePos = pos;
    }

    void CameraControlService::SetCameraPosition(const Vector2f &pos)
    {
        GetCamera().SetPosition(pos);
    }

    void CameraControlService::SetZoomLevel(float level)
    {
        zoomLevel = level;
        GetCamera().SetZoom( CalcZoom(zoomLevel) );
    }

    float CameraControlService::GetZoomLevel() const
    {
        return zoomLevel;
    }

    float CameraControlService::CalcZoom(float level)
    {
        return std::pow(ZoomStep, level);
    }

    void CameraControlService::SetHomeZoom(float level)
    {
        homeZoom = level;
    }

} // end of namespace