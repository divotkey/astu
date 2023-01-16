/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Extensions/ObjectTypeBuilder.h"
#include "Velox/Extensions/GraphicsInstants.h"
#include "Velox/Interpreter/VeloxExtension.h"

namespace astu {

    class AstuExtension : public velox::VeloxExtension {
    public:

        AstuExtension();

        // Inherited via VeloxExtension
        void InjectExtension(velox::Interpreter &interpreter) const override;

    private:
    };

} // end of namespace