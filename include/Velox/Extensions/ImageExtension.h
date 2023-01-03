/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/VeloxExtension.h"

namespace astu {

    class ImageExtension : public velox::VeloxExtension {
    public:

        ImageExtension();

        // Inherited via VeloxExtension
        void InjectExtension(velox::Interpreter &interpreter) const override;
    };

} // end of namespace