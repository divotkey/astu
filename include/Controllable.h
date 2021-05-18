/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>
#include <vector>

// Local includes
#include "Controller.h"

namespace astu {


    class Controllable {
    public:

        /** Virtual destructor. */
        virtual ~Controllable() {}

        void AttachController(std::shared_ptr<Controller> ctrl);
        void DetachController(std::shared_ptr<Controller> ctrl);
        void DetachAllController();
        bool HasController(std::shared_ptr<Controller> ctrl);
        void UpdateControllers(double dt);

    private:
        /** The controllers attached to this object. */
        std::vector<std::shared_ptr<Controller>> controllers;
    };

} // end of namespace