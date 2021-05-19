/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard includes
#include <stdexcept>
#include <iostream>

// Local includes
#include "IWindowManager.h"
#include "Camera2Service.h"

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// Camera2
    /////////////////////////////////////////////////

    Camera2::Camera2() 
    {
        Reset();
    }

    Camera2& Camera2::SetPosition(float x, float y)
    {
        position.Set(x, y);
        dirty = invDirty = true;
        return *this;
    }

    Camera2& Camera2::SetOrientation(float phi)
    {
        orientation = phi;
        dirty = invDirty = true;
        return *this;
    }

    const Matrix3f& Camera2::GetMatrix() const
    {
        if (dirty) {
            matrix.SetToIdentity();
            matrix.Translate(-position);
            matrix.Rotate(-orientation);
            matrix.Scale(scaling);
            matrix.Translate(targetWidth * 0.5f, targetHeight * 0.5f);
            dirty = false;
        }

        return matrix;
    }

    const Matrix3f& Camera2::GetInverseMatrix() const
    {
        if (invDirty) {
            invMatrix = GetMatrix();
            invMatrix.Invert();
            invDirty = false;
        }

        return invMatrix;
    }

    Camera2& Camera2::Reset()
    {
        ShowScreenSpace();
        SetPosition(0, 0);
        SetOrientation(0);
        SetRenderTargetSize(0, 0);

        return *this;
    }

    Camera2& Camera2::ShowScreenSpace()
    {
        state = make_unique<ScreenSpaceState>();
        return *this;
    }

    Camera2& Camera2::ShowFixedWorldWidth(float w)
    {
        state = make_unique<FixedWidthState>(w); 
        return *this;
    }

    Camera2& Camera2::ShowFixedWorldHeight(float h)
    {
        state = make_unique<FixedHeightState>(h);        
        return *this;
    }

    void Camera2::SetRenderTargetSize(int width, int height)
    {
        targetWidth = static_cast<float>(width); 
        targetHeight = static_cast<float>(height);
        state->UpdateScaling(*this);
        dirty = invDirty = true;
    }

    void Camera2::ScreenSpaceState::UpdateScaling(Camera2 & cam)
    {
        cam.scaling.Set(1, 1);
    }

    void Camera2::FixedWidthState::UpdateScaling(Camera2 & cam)
    {
        float s = cam.targetWidth / worldWidth;
        cam.scaling.Set(s, s);
    }

    void Camera2::FixedHeightState::UpdateScaling(Camera2 & cam)
    {
        float s = cam.targetHeight / worldHeight;
        cam.scaling.Set(s, s);
    }


    /////////////////////////////////////////////////
    /////// Camera2Service
    /////////////////////////////////////////////////

    const string Camera2Service::DEFAULT_CAMERA = "Default Cam";

    Camera2Service::Camera2Service()
        : Service("Camera 2D Manager")
    {
        CreateCamera(DEFAULT_CAMERA);
    }

    std::shared_ptr<Camera2> Camera2Service::CreateCamera(const std::string & name)
    {
        auto it = cameraMap.find(name);
        if (it != cameraMap.end()) {
            throw std::logic_error("Camera '" + name + "' alreayd exists");
        }

        auto result = make_shared<Camera2>();
        cameraMap[name] = result;

        const auto& wnd = ASTU_SERVICE(IWindowManager);
        result->SetRenderTargetSize(wnd.GetWidth(), wnd.GetHeight());
        return result;
    }

    std::shared_ptr<Camera2> Camera2Service::GetCamera(const std::string & name)
    {
        auto it = cameraMap.find(name);
        if (it == cameraMap.end()) {
            throw std::logic_error("Camera '" + name + "' is unknown");
        }

        return it->second;
    }

    bool Camera2Service::HasCamera(const std::string & name) const
    {
        return cameraMap.find(name) == cameraMap.end();
    }

    void Camera2Service::OnStartup()
    {
        const auto& wnd = ASTU_SERVICE(IWindowManager);
        OnResize(wnd.GetWidth(), wnd.GetHeight());
    }

    void Camera2Service::OnShutdown()
    {
        // Intentionally left empty.
    }

    void Camera2Service::OnResize(int width, int height) 
    {
        for (auto it : cameraMap) {
            it.second->SetRenderTargetSize(width, height);
        }
    }

} // end of namespace