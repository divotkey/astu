/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Suite2D/CameraControlService.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <iostream>
#include <cmath>
using namespace std;

#define ZOOM_STEP 1.1f

namespace astu::suite2d {

    CameraControlService::CameraControlService(int updatePriority)
        : Service("Camera 2D Control Service")
        , Updatable(updatePriority)
        , mouseButton(Mouse::Button::RIGHT)
        , homeKey(Keyboard::KEY_HOME)
    {
        // Intentionally left empty.
    }

    void CameraControlService::OnStartup()
    {
        dragging = false;
        zoomLevel = 0;
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
            GetCamera().SetPosition(0, 0);
            GetCamera().SetZoom(1.0f);
            zoomLevel = 0;
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
        zoomLevel += amount;
        GetCamera().SetZoom( std::pow(ZOOM_STEP, zoomLevel) );
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

} // end of namespace