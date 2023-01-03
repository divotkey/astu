/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2022-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Service/Service.h"
#include "Service/LoggingService.h"
#include "Velox/Interpreter/VeloxExtension.h"

// C++ Standard Library includes
#include <memory>
#include <vector>


namespace astu {

    /**
     * This service manages Velox extensions.
     *
     * @ingroup velox_group
     */
    class VeloxExtensionService : virtual public Service, public LoggingClient {
    public:

        /**
         * Constructor.
         */
        VeloxExtensionService();

        bool HasExtension(std::shared_ptr<velox::VeloxExtension> extension) const;
        void AddExtension(std::shared_ptr<velox::VeloxExtension> extension);
        void RemoveExtension(std::shared_ptr<velox::VeloxExtension> extension);
        void InjectExtensions(velox::Interpreter &interpreter);

    private:
        std::vector<std::shared_ptr<velox::VeloxExtension>> extensions;
    };

} // end of namespace