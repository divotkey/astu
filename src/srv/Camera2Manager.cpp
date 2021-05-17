/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard includes
#include <stdexcept>

// Local includes
#include "Camera2Manager.h"

using namespace std;

namespace astu {

    /////////////////////////////////////////////////
    /////// Camera2
    /////////////////////////////////////////////////

    Camera2::Camera2() 
        // : position(0, 0), angle(0), dirty(true)
    {
        // Intentionally left empty
    }

    // const Matrix3f& Camera2::GetTransform() const
    // {
    //     return transform;
    // }

    /////////////////////////////////////////////////
    /////// Camera2Manager
    /////////////////////////////////////////////////

    const string Camera2Manager::DEFAULT_CAMERA = "Default Cam";

    Camera2Manager::Camera2Manager()
        : Service("Camera 2D Manager")
    {
        // Intentionally left empty
    }

    std::shared_ptr<Camera2> Camera2Manager::CreateCamera(const std::string & name)
    {
        auto it = cameraMap.find(name);
        if (it != cameraMap.end()) {
            throw std::logic_error("Camera '" + name + "' alreayd exists");
        }

        auto result = make_shared<Camera2>();
        cameraMap[name] = result;

        return result;
    }

    std::shared_ptr<Camera2> Camera2Manager::GetCamera(const std::string & name)
    {
        auto it = cameraMap.find(name);
        if (it == cameraMap.end()) {
            throw std::logic_error("Camera '" + name + "' is unknown");
        }

        return it->second;
    }

    bool Camera2Manager::HasCamera(const std::string & name) const
    {
        return cameraMap.find(name) == cameraMap.end();
    }


} // end of namespace