/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Util/Controllable.h"

// C++ Standard Library includes
#include <stdexcept>
#include <algorithm>

using namespace std;

namespace astu {

    bool Controllable::HasController(std::shared_ptr<Controller> ctrl)
    {
        return find(controllers.begin(), controllers.end(), ctrl) != controllers.end();
    }

    void Controllable::AttachController(std::shared_ptr<Controller> ctrl)
    {
        if (HasController(ctrl)) {
            throw logic_error("Controller has already been attached");
        }

        if (ctrl->HasControllable()) {
            ctrl->GetControllable()->DetachController(ctrl);
        }

        ctrl->SetControllable(this);
        controllers.push_back(ctrl);
    }

    void Controllable::DetachController(std::shared_ptr<Controller> ctrl)
    {
        if (!HasController(ctrl)) {
            return;
        }
        ctrl->SetControllable(nullptr);
        controllers.erase(
            remove(controllers.begin(), controllers.end(), ctrl), 
            controllers.end());
    }

    void Controllable::DetachAllController()
    {
        for (const auto & ctrl : controllers) {
            ctrl->SetControllable(nullptr);
        }
        controllers.clear();
    }

    void Controllable::UpdateControllers(double dt)
    {
        for (const auto & ctrl : controllers) {
            ctrl->Update(dt);
        }
    }

} // end of namespace