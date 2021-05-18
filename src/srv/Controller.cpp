/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// C++ Standard Library includes
#include <memory>

// Local includes
#include "Controller.h"

namespace astu {

    Controller::Controller() : controllable(nullptr)
    {
        // Intentionally left empty.        
    }

    void Controller::SetControllable(Controllable *controllable)
    {
        this->controllable = controllable;
    }

    void Controller::Update(double dt)
    {
        // Intentionally left empty.        
    }

} // end of namespace