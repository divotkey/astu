/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Font.h"
#include "Service/UpdateService.h"
#include "Service/TimeService.h"

namespace astu {

    class SdlFontFactory final
        : public virtual Service
        , public FontFactory
    {
    public:

        /**
         * Constructor.
         */
        SdlFontFactory();

        // Inherited via FontFactory
        std::shared_ptr<Font> LoadFont(const std::string &fontFile, int pointSize) const override;

    protected:
        void OnStartup() override;

        void OnShutdown() override;
    };

} // end of namespace

