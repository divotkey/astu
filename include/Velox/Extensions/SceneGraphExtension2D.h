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
#include "Suite2D/CommandService2D.h"
#include "Service/ProgressSignal.h"

namespace astu {

    class SceneGraphExtension2D : public velox::VeloxExtension {
    public:

        /**
         * Constructor.
         */
        SceneGraphExtension2D();

        // Inherited via VeloxExtension
        void InjectExtension(velox::Interpreter &interpreter) const override;

    private:
        mutable std::shared_ptr<suite2d::CommandService2D> cmdSrv;
        mutable std::shared_ptr<suite2d::SceneGraph> sceneGraph;
        mutable std::shared_ptr<VertexBufferBuilder2f> vtxBufferBuilder;
        mutable std::shared_ptr<ProgressSignalService> progressSignalSrv;

        void AddCommonSpatialFunctions(ObjectTypeBuilder &builder) const;
        void AddCameraFunctions(velox::Interpreter &interpreter) const;

    };

} // end of namespace