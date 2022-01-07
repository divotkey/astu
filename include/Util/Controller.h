/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// C++ Standard Library includes
#include <memory>

namespace astu {

    // Forward declaration
    class Controllable;

    class Controller {
    public:

        /**
         * Constructor.
         */
        Controller();

        /** Virtual destructor. */
        virtual ~Controller() {}

        void SetControllable(Controllable *controllable);

        Controllable* GetControllable() {
            return controllable;
        }
        const Controllable* GetControllable() const {
            return controllable;
        }

        bool HasControllable() const {
            return controllable != nullptr;
        }

        virtual void Update(double dt);

    private:
        /** The controllable controlled to be controlled. */
        Controllable* controllable;
    };

} // end of namespace