/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Parser/Source.h"

// C++ Standard Library includes
#include <sstream>
#include <string>

namespace velox {

    class SourceString : public Source {
    public:

        /**
         * Constructor.
         *
         * @param inSource  the string representing the source
         */
        SourceString(const std::string& inSource);

    protected:
        // Inherited via Source
        std::shared_ptr<std::istream> GetStream() override;

    private:
        /** The actual source code. */
        std::string source;
    };

} // end of namespace



