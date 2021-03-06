/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

 // Local includes
#include "CameraService2D.h"
#include "IWindowManager.h"

 // C++ Standard includes
#include <stdexcept>
#include <iostream>

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// Camera2
    /////////////////////////////////////////////////

    Camera2D::Camera2D() 
    {
        Reset();
    }

    Camera2D& Camera2D::SetPosition(float x, float y)
    {
        position.Set(x, y);
        dirty = invDirty = true;
        return *this;
    }

    const astu::Vector2f Camera2D::GetPosition() const
    {
        return position;
    }

    Camera2D& Camera2D::SetOrientation(float phi)
    {
        orientation = phi;
        dirty = invDirty = true;
        return *this;
    }

    Camera2D& Camera2D::SetZoom(float z)
    {
        zoom = z;
        dirty = invDirty = true;
        return *this;
    }

    float Camera2D::GetZoom() const
    {
        return zoom;
    }

    float Camera2D::GetOrientation() const
    {
        return orientation;
    }

    const Matrix3f& Camera2D::GetMatrix() const
    {
        if (dirty) {
            matrix.SetToIdentity();
            matrix.Translate(-position);
            matrix.Rotate(-orientation);
            matrix.Scale(scaling * zoom);
            matrix.Translate(targetWidth * 0.5f, targetHeight * 0.5f);
            dirty = false;
        }

        return matrix;
    }

    const Matrix3f& Camera2D::GetInverseMatrix() const
    {
        if (invDirty) {
            invMatrix = GetMatrix();
            invMatrix.Invert();
            invDirty = false;
        }

        return invMatrix;
    }

    Camera2D& Camera2D::Reset()
    {
        ShowScreenSpace();
        SetPosition(0, 0);
        SetOrientation(0);
        SetRenderTargetSize(0, 0);
        zoom = 1.0;

        return *this;
    }

    Camera2D& Camera2D::ShowScreenSpace()
    {
        SwitchState(make_unique<ScreenSpaceState>());
        return *this;
    }

    Camera2D& Camera2D::ShowFixedWidth(float w)
    {
        SwitchState(make_unique<FixedWidthState>(w));
        return *this;
    }

    Camera2D& Camera2D::ShowFixedHeight(float h)
    {
        SwitchState(make_unique<FixedHeightState>(h));
        return *this;
    }

    Camera2D& Camera2D::ShowStreched(float width, float height)
    {
        SwitchState(make_unique<StrechedState>(width, height));
        return *this;
    }

    Camera2D& Camera2D::ShowFitting(float width, float height)
    {
        SwitchState(make_unique<FittingState>(width, height));
        return *this;
    }

    Camera2D& Camera2D::ShowFilling(float width, float height)
    {
        SwitchState(make_unique<FillingState>(width, height));
        return *this;
    }

    void Camera2D::SetRenderTargetSize(int width, int height)
    {
        targetWidth = static_cast<float>(width); 
        targetHeight = static_cast<float>(height);
        state->UpdateScaling(*this);
        dirty = invDirty = true;
    }

    void Camera2D::ScreenSpaceState::UpdateScaling(Camera2D & cam)
    {
        cam.scaling.Set(1, 1);
    }

    void Camera2D::FixedWidthState::UpdateScaling(Camera2D & cam)
    {
        float s = cam.targetWidth / worldWidth;
        cam.scaling.Set(s, s);
    }

    void Camera2D::FixedHeightState::UpdateScaling(Camera2D & cam)
    {
        float s = cam.targetHeight / worldHeight;
        cam.scaling.Set(s, s);
    }

    void Camera2D::StrechedState::UpdateScaling(Camera2D & cam)
    {
        cam.scaling.Set(
            cam.targetWidth / worldWidth, 
            cam.targetHeight / worldHeight);
    }

    void Camera2D::FittingState::UpdateScaling(Camera2D & cam)
    {
        float s;
        if (cam.targetWidth / cam.targetHeight < ar) {
            s = cam.targetWidth / worldWidth;
        } else {
            s = cam.targetHeight / worldHeight;
        }
        cam.scaling.Set(s, s);
    }

    void Camera2D::FillingState::UpdateScaling(Camera2D & cam)
    {
        float s;
        if (cam.targetWidth / cam.targetHeight < ar) {
            s = cam.targetHeight / worldHeight;
        } else {
            s = cam.targetWidth / worldWidth;
        }
        cam.scaling.Set(s, s);
    }

    void Camera2D::SwitchState(std::unique_ptr<CameraState> newState)
    {
        state = std::move(newState);
        state->UpdateScaling(*this);
        dirty = invDirty = true;
    }

    /////////////////////////////////////////////////
    /////// Camera2Service
    /////////////////////////////////////////////////

    const string CameraService2D::DEFAULT_CAMERA = "Default Cam";

    CameraService2D::CameraService2D()
        : Service("Camera 2D Manager")
    {
        CreateCamera(DEFAULT_CAMERA);
    }

    std::shared_ptr<Camera2D> CameraService2D::CreateCamera(const std::string & camName)
    {
        auto it = cameraMap.find(camName);
        if (it != cameraMap.end()) {
            throw std::logic_error("Camera '" + camName + "' already exists");
        }

        auto result = make_shared<Camera2D>();
        cameraMap[camName] = result;

        const auto& wnd = ASTU_SERVICE(IWindowManager);
        result->SetRenderTargetSize(wnd.GetWidth(), wnd.GetHeight());
        return result;
    }

    std::shared_ptr<Camera2D> CameraService2D::GetCamera(const std::string & camName)
    {
        auto it = cameraMap.find(camName);
        if (it == cameraMap.end()) {
            throw std::logic_error("Camera '" + camName + "' is unknown");
        }

        return it->second;
    }

    bool CameraService2D::HasCamera(const std::string & camName) const
    {
        return cameraMap.find(camName) == cameraMap.end();
    }

    void CameraService2D::OnStartup()
    {
        const auto& wnd = ASTU_SERVICE(IWindowManager);
        OnResize(wnd.GetWidth(), wnd.GetHeight());
    }

    void CameraService2D::OnShutdown()
    {
        // Intentionally left empty.
    }

    bool CameraService2D::OnResize(int width, int height) 
    {
        for (auto it : cameraMap) {
            it.second->SetRenderTargetSize(width, height);
        }
        
        return false;
    }

    /////////////////////////////////////////////////
    /////// Camera2Client
    /////////////////////////////////////////////////

    CameraClient2D::CameraClient2D(const std::string & cameraName, bool createCamera)
        : createCamera(createCamera)
		, cameraName(cameraName)
    {
        AddStartupHook([this](){ InitCamera(); });
        AddShutdownHook([this](){ camera = nullptr; });
    }

    void CameraClient2D::UseCamera(const std::string & camName, bool create)
    {
        cameraName = camName;
        createCamera = create;
        if (GetStatus() == Running) {
            InitCamera();
        }
    }

    const std::string& CameraClient2D::GetCameraName() const
    {
        return cameraName;
    }

    void CameraClient2D::InitCamera()
    {
        if (!ASTU_HAS_SERVICE(CameraService2D)) {
            // Fallback
            camera = make_shared<Camera2D>();
            return;
        }

        if (createCamera) {
            camera = ASTU_SERVICE(CameraService2D).GetOrCreateCamera(cameraName);
        } else {
            camera = ASTU_SERVICE(CameraService2D).GetCamera(this->cameraName);
        }
    }

} // end of namespace