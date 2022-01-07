/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Util/Controller.h"

// C++ Standard Library includes
#include <memory>

namespace astu {

    Controller::Controller() : controllable(nullptr)
    {
        // Intentionally left empty.        
    }

    void Controller::SetControllable(Controllable *c)
    {
        controllable = c;
    }

    void Controller::Update(double dt)
    {
        // Intentionally left empty.        
    }

} // end of namespace