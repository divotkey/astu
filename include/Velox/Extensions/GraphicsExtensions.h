/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Extensions/ObjectTypeBuilder.h"
#include "Velox/Interpreter/VeloxExtension.h"

namespace astu {

    class GraphicsExtension : public velox::VeloxExtension {
    public:

        GraphicsExtension();

        // Inherited via VeloxExtension
        void InjectExtension(velox::Interpreter &interpreter) const override;

    private:
        void AddCommonPatternFunctions(ObjectTypeBuilder &builder) const;
    };


} // end of namespace