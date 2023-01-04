/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include <Velox/Interpreter/Interpreter.h>
#include <Velox/Interpreter/InterpreterInstantDefinition.h>

// C++ Standard Library includes
#include <string>
#include <memory>
#include <map>

namespace astu {

    class InstantBuilder {
    public:

        InstantBuilder();

        InstantBuilder &AddInstant(const std::string & instantSource);
        InstantBuilder &Reset();
        bool HasInstant(const std::string &name) const;
        void Build(velox::Interpreter &interpreter);

    private:
        /** The instant definitions. */
        std::vector<std::shared_ptr<velox::InterpreterInstantDefinition>> instants;
    };

} // end of namespace
