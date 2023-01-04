/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterStatement.h"
#include "Velox/Interpreter/InterpreterFunctionDefinition.h"

// C++ Standard Library includes
#include <string>
#include <vector>
#include <memory>

namespace velox {

    class InterpreterClassDefinition : public InterpreterStatement {
    public:

        /**
         * Constructor.
         *
         * @param name          the name of the class
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterClassDefinition(const std::string & name, unsigned int lineNumber)
            : InterpreterStatement(lineNumber), typeName(name) {}

        void AddFunction(std::shared_ptr<InterpreterFunctionDefinition> function);
        bool HasFunction(const std::string& name) const;

        void Execute(ScriptContext &sc) override;

        void Prepare(ScriptContext &sc) override;

    private:
        std::string typeName;
        std::vector<std::shared_ptr<InterpreterFunctionDefinition>> functions;
    };

}


