/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

 // Local includes
#include "Suite2D/CameraService.h"
#include "Service/WindowService.h"

 // C++ Standard includes
#include <stdexcept>
#include <iostream>

using namespace std;

namespace astu::suite2d {

    /////////////////////////////////////////////////
    /////// Camera
    /////////////////////////////////////////////////

    Camera::Camera() 
    {
        Reset();
    }

    Camera& Camera::SetPosition(float x, float y)
    {
        position.Set(x, y);
        dirty = invDirty = true;
        return *this;
    }

    const astu::Vector2f Camera::GetPosition() const
    {
        return position;
    }

    Camera& Camera::SetOrientation(float phi)
    {
        orientation = phi;
        dirty = invDirty = true;
        return *this;
    }

    Camera& Camera::SetZoom(float z)
    {
        zoom = z;
        dirty = invDirty = true;
        return *this;
    }

    float Camera::GetZoom() const
    {
        return zoom;
    }

    float Camera::GetOrientation() const
    {
        return orientation;
    }

    float Camera::GetViewWidth(bool includeZoom) const
    {
        if (includeZoom) {
            return targetWidth / (scaling.x * zoom);        
        } else{
            return targetWidth / scaling.x;        
        }
    }

    float Camera::GetViewHeight(bool includeZoom) const
    {
        if (includeZoom) {
            return targetHeight / (scaling.y * zoom);        
        } else{
            return targetHeight / scaling.y;        
        }
    }

    const Matrix3f& Camera::GetMatrix() const
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

    const Matrix3f& Camera::GetInverseMatrix() const
    {
        if (invDirty) {
            invMatrix = GetMatrix();
            invMatrix.Invert();
            invDirty = false;
        }

        return invMatrix;
    }

    Camera& Camera::Reset()
    {
        ShowScreenSpace();
        SetPosition(0, 0);
        SetOrientation(0);
        SetRenderTargetSize(0, 0);
        zoom = 1.0;

        return *this;
    }

    Camera& Camera::ShowScreenSpace()
    {
        SwitchState(make_unique<ScreenSpaceState>());
        return *this;
    }

    Camera& Camera::ShowFixedWidth(float w)
    {
        SwitchState(make_unique<FixedWidthState>(w));
        return *this;
    }

    Camera& Camera::ShowFixedHeight(float h)
    {
        SwitchState(make_unique<FixedHeightState>(h));
        return *this;
    }

    Camera& Camera::ShowStreched(float width, float height)
    {
        SwitchState(make_unique<StrechedState>(width, height));
        return *this;
    }

    Camera& Camera::ShowFitting(float width, float height)
    {
        SwitchState(make_unique<FittingState>(width, height));
        return *this;
    }

    Camera& Camera::ShowFilling(float width, float height)
    {
        SwitchState(make_unique<FillingState>(width, height));
        return *this;
    }

    void Camera::SetRenderTargetSize(int width, int height)
    {
        targetWidth = static_cast<float>(width); 
        targetHeight = static_cast<float>(height);
        state->UpdateScaling(*this);
        dirty = invDirty = true;
    }

    void Camera::ScreenSpaceState::UpdateScaling(Camera & cam)
    {
        cam.scaling.Set(1, 1);
    }

    void Camera::FixedWidthState::UpdateScaling(Camera & cam)
    {
        float s = cam.targetWidth / worldWidth;
        cam.scaling.Set(s, s);
    }

    void Camera::FixedHeightState::UpdateScaling(Camera & cam)
    {
        float s = cam.targetHeight / worldHeight;
        cam.scaling.Set(s, s);
    }

    void Camera::StrechedState::UpdateScaling(Camera & cam)
    {
        cam.scaling.Set(
            cam.targetWidth / worldWidth, 
            cam.targetHeight / worldHeight);
    }

    void Camera::FittingState::UpdateScaling(Camera & cam)
    {
        float s;
        if (cam.targetWidth / cam.targetHeight < ar) {
            s = cam.targetWidth / worldWidth;
        } else {
            s = cam.targetHeight / worldHeight;
        }
        cam.scaling.Set(s, s);
    }

    void Camera::FillingState::UpdateScaling(Camera & cam)
    {
        float s;
        if (cam.targetWidth / cam.targetHeight < ar) {
            s = cam.targetHeight / worldHeight;
        } else {
            s = cam.targetWidth / worldWidth;
        }
        cam.scaling.Set(s, s);
    }

    void Camera::SwitchState(std::unique_ptr<CameraState> newState)
    {
        state = std::move(newState);
        state->UpdateScaling(*this);
        dirty = invDirty = true;
    }

    /////////////////////////////////////////////////
    /////// CameraService
    /////////////////////////////////////////////////

    const string CameraService::DEFAULT_CAMERA = "Default Cam";

    CameraService::CameraService()
        : Service("Camera 2D Manager")
    {
        CreateCamera(DEFAULT_CAMERA);
    }

    std::shared_ptr<Camera> CameraService::CreateCamera(const std::string & camName)
    {
        auto it = cameraMap.find(camName);
        if (it != cameraMap.end()) {
            throw std::logic_error("Camera '" + camName + "' already exists");
        }

        auto result = make_shared<Camera>();
        cameraMap[camName] = result;

        const auto& wnd = ASTU_SERVICE(WindowService);
        result->SetRenderTargetSize(wnd.GetWidth(), wnd.GetHeight());
        return result;
    }

    std::shared_ptr<Camera> CameraService::GetCamera(const std::string & camName)
    {
        auto it = cameraMap.find(camName);
        if (it == cameraMap.end()) {
            throw std::logic_error("Camera '" + camName + "' is unknown");
        }

        return it->second;
    }

    bool CameraService::HasCamera(const std::string & camName) const
    {
        return cameraMap.find(camName) == cameraMap.end();
    }

    void CameraService::OnStartup()
    {
        const auto& wnd = ASTU_SERVICE(WindowService);
        OnResize(wnd.GetWidth(), wnd.GetHeight());
    }

    void CameraService::OnShutdown()
    {
        // Intentionally left empty.
    }

    bool CameraService::OnResize(int width, int height) 
    {
        for (auto it : cameraMap) {
            it.second->SetRenderTargetSize(width, height);
        }
        
        return false;
    }

    /////////////////////////////////////////////////
    /////// CameraClient
    /////////////////////////////////////////////////

    CameraClient::CameraClient(const std::string & cameraName, bool createCamera)
        : createCamera(createCamera)
		, cameraName(cameraName)
    {
        AddStartupHook([this](){ InitCamera(); });
        AddShutdownHook([this](){ camera = nullptr; });
    }

    void CameraClient::UseCamera(const std::string & camName, bool create)
    {
        cameraName = camName;
        createCamera = create;
        if (GetStatus() == Running) {
            InitCamera();
        }
    }

    const std::string& CameraClient::GetCameraName() const
    {
        return cameraName;
    }

    void CameraClient::InitCamera()
    {
        if (!ASTU_HAS_SERVICE(CameraService)) {
            // Fallback
            camera = make_shared<Camera>();
            return;
        }

        if (createCamera) {
            camera = ASTU_SERVICE(CameraService).GetOrCreateCamera(cameraName);
        } else {
            camera = ASTU_SERVICE(CameraService).GetCamera(this->cameraName);
        }
    }

} // end of namespace