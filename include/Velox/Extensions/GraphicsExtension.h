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

    class GraphicsExtension : public velox::VeloxExtension {
    public:

        GraphicsExtension();

        // Inherited via VeloxExtension
        void InjectExtension(velox::Interpreter &interpreter) const override;

    private:
        GraphicsInstants graphicsInstants;

        void AddCoreObjects(velox::Interpreter &interpreter) const;
        void AddPalette(velox::Interpreter &interpreter) const;
        void AddCommonPatternFunctions(ObjectTypeBuilder &builder) const;
        void AddPatternInstants(velox::Interpreter &interpreter) const;
        void AddWebColors(velox::Interpreter &interpreter) const;
        void AddRalColors(velox::Interpreter &interpreter) const;
        void AddCorePatterns(velox::Interpreter &interpreter) const;
        void AddCompositePatterns(velox::Interpreter &interpreter) const;
        void AddRenderers(velox::Interpreter &interpreter) const;
        void AddMiscellaneous(velox::Interpreter &interpreter) const;
    };

} // end of namespace