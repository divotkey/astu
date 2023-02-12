/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Controller.h"
#include "Velox/Interpreter/ItemData.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    class Controllable : public velox::ItemData {
    public:

        /** Virtual destructor. */
        virtual ~Controllable() = default;

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