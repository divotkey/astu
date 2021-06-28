/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020, 2021 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

// Local includes
#include "Service.h"

// C++ Standard Library includes
#include <memory>
#include <string>

namespace astu {

    class AxisBinding {
    public:

        float GetValue() const;

    private:
    };

    class ActionBinding {
    public:

    }

    class InputMapperService : public virtual Service {
    public:

        std::shared_ptr<AxisBinding> GetAxisBinding(const std::string & name);

    private:
    };

};