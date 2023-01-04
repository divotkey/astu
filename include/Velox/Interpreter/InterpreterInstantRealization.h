/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/InterpreterExpression.h"

// C++ Standard Library includes
#include <vector>
#include <memory>

namespace velox {

    class  InterpreterInstantRealization : public InterpreterExpression {
    public:

        /**
         * Constructor.
         *
         * @param lineNumber    information about the position of this statement within the source code
         */
        InterpreterInstantRealization(unsigned int lineNumber);

        void SetTypeName(const std::string& name);
        void AddStatement(std::shared_ptr<InterpreterStatement> statement);

        // Inherited via InterpreterExpression
        std::shared_ptr<Item> Evaluate(ScriptContext &sc) override;

    private:
        /** The name of the type of the object which should be created. */
        std::string typeName;

        /** The statements this statement block contains. */
        std::vector<std::shared_ptr<InterpreterStatement>> statements;
    };

}



